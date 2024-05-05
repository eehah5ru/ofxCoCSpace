//
//  videoPlayers.cpp
//  deSerialKeyer
//
//  Created by nicola spesivcev on 08.11.14.
//
//

#include <stdio.h>
#include "videoPlayers.h"

videoPlayers::videoPlayers () {
    
}


videoPlayers::~videoPlayers() {
    free();
}


void videoPlayers::initPlayers(string basePath) {
    LOG_VP() << "initPlayers: with basePath:" << basePath;
    _basePath = basePath;

    if (!goToFolder(_currentDirIndex)) {
        throw runtime_error("initPlayers: failed");
    }

    _initialized = true;

    LOG_VP() << "initPlayers: done";
}


void videoPlayers::free () {
    for (auto i: _players) {
        delete i;
    }

    _players.clear();
}

ofVideoPlayer* videoPlayers::current() {
    if (_players.size() == 0) {
        LOG_VP_ERROR() << "current: no current video!";
        throw out_of_range("current: no current video");
    }

    if (_players.size() <= _curPlayerIndex) {
        LOG_VP_ERROR() << "current: _curPlayerIndex is too big";
        LOG_VP_ERROR() << "_curPlayerIndex = " << _curPlayerIndex;
        LOG_VP_ERROR() << "videos count: " << _players.size();
        throw out_of_range("current: _curPlayerIndex is too big");
    }

    return _players.at(_curPlayerIndex);
}

string videoPlayers::getCurrentMovieName () {
    ofFile f = ofFile(current()->getMoviePath());
    
    return f.getFileName();
}

void videoPlayers::update () {
    //_videoPlayers.current()->update();

    if (!_isPlaying) {
        return;
    }

    //LOG_VP_NOTICE() << "update: video position: " << current()->getPosition() << ", speed: " << current()->getSpeed();

    if (!current()->isLoaded()) {
      return;
    }
   
   // prepare new movie
    if (ofInRange(current()->getPosition(), 0.8, 0.9)) {
        _needToSwapMovies = true;
    }
    
    if (ofInRange(current()->getPosition(), 0.99, 1)) {
        if (_needToSwapMovies) {
            if ((!goToNextVideo() || goToVideo(0))) {
                LOG_VP_WARNING() << "update: failed to go to the next video";
            }
        }
    }
    
    if (current()->isLoaded() && _isPlaying && !current()->isPlaying()) {
      LOG_VP_NOTICE() << "update: force starting video";
        start();
    }

    // if (_isPlaying) {
    //     current()->play();
    // } else {
    //     current()->stop();
    // }

    
    current()->update();
}

void videoPlayers::start () {
    if (_isPlaying && current()->isPlaying()) {
        return;
    }

    // silently sync with global players state
    if (_isPlaying) {
        current()->play();
        current()->setPosition(0.0f);	
        return;
    }

    LOG_VP_NOTICE() << "start playing video: " << getCurrentMovieName();

    _isPlaying = true;
    current()->play();
    current()->setPosition(0.0f);
    
}

void videoPlayers::stop () {
    if (_isPlaying) {
        LOG_VP_NOTICE() << "stop video: " << getCurrentMovieName();
        _isPlaying = false;
        current()->stop();
        return;
    }

    if (current()->isPlaying()) {
        current()->stop();
    }

}

void videoPlayers::togglePlay () {
    if (_isPlaying) {
        stop();
        return;
    } 
    start();
}   
 


void videoPlayers::changePlayer() {
    LOG_VP_NOTICE() << "rendomely changing video";
    goToVideo(ofRandom(0, _players.size()));
}


void videoPlayers::addVideoPlayer (const string& path) {
    LOG_VP_NOTICE() << "add video: " + path;

    ofVideoPlayer* result = new ofVideoPlayer();
    result->load(path);
    result->setLoopState(OF_LOOP_NONE);
    // mute
    result->setVolume(0.0f);
    result->setSpeed(1.0f);
    result->play();
    result->stop();
    // result->setPosition(0.0f);
    
    _players.push_back(result);
}

// change directory and reload videos
bool videoPlayers::goToFolder(int dirIndex) {
    LOG_VP() << "goToFolder: searching for videos in " << mkFolderPath(dirIndex);
    
    if (!hasFolder(dirIndex)) {
        LOG_VP_ERROR() << "dir does not exist: " << mkFolderPath(dirIndex);
        return false;
    }
    
    ofDirectory dir;
    dir.open(mkFolderPath(dirIndex));

    dir.allowExt("mov");
    dir.allowExt("mp4");
    dir.allowExt("m4v");

    
    dir.listDir(); 

    if (dir.size() == 0) {
        LOG_VP_ERROR() << "video data directory is empty!";
        return false;
        //throw std::runtime_error("video data directory is empty!");
    }

    if (_initialized) {
        current()->stop();
        free();
    }

    for (auto i: dir.getFiles()) {
        addVideoPlayer(i.getAbsolutePath());
    }

    LOG_VP_NOTICE() << "videos to play: " << _players.size(); 

    _currentDirIndex = dirIndex;
    goToVideo(0);

    return true;
}

bool videoPlayers::goToVideo(int newPlayerIndex) {
    if (!hasVideo(newPlayerIndex)) {
        LOG_VP_WARNING() << "goToVideo: no video with index " << newPlayerIndex;
        return false;
    }

    if (current()->isPlaying()) {
        current()->stop();
    }

    _curPlayerIndex = newPlayerIndex;
    _needToSwapMovies = false;

    LOG_VP_NOTICE() << "goToVideo: current video index: " << newPlayerIndex;

    return true;
}

bool videoPlayers::goToNextVideo() {
    if (!hasVideo(_curPlayerIndex + 1)) {
        LOG_VP_WARNING() << "goToNextVideo: already last video";
        return false;
    }

    return goToVideo(_curPlayerIndex + 1);
}

bool videoPlayers::goToPrevVideo() {
    if (!hasVideo(_curPlayerIndex - 1)) {
        LOG_VP_WARNING() << "goToPrevVideo: already first video";
        return false;
    }

    return goToVideo(_curPlayerIndex - 1);
}

bool videoPlayers::goToNextFolder() {
    if (!hasFolder(_currentDirIndex + 1)) {
        LOG_VP_ERROR() << "goToNextFolder: already in the last folder";
        return false;
    }

    return goToFolder(_currentDirIndex + 1);
}

bool videoPlayers::goToPrevFolder() {
    if (!hasFolder(_currentDirIndex - 1)) {
        LOG_VP_ERROR() << "goToPrevFolder: already in the first folder";
        return false;
    }

    return goToFolder(_currentDirIndex - 1);
}

string videoPlayers::mkFolderPath(int dirIndex) {
    return _basePath + "_" + std::to_string(dirIndex);
}

bool videoPlayers::hasFolder (int dirIndex) {
    ofDirectory dir(mkFolderPath(dirIndex));

    return dir.exists();
}

bool videoPlayers::hasVideo(int playerIndex) {
    if (playerIndex < 0) {
        return false;
    }

    if (_players.size() <= playerIndex) {
        return false;
    }

    return true;
}

