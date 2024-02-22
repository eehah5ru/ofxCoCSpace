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
   
   // prepare new movie
    if (ofInRange(current()->getPosition(), 0.8, 0.9)) {
        _needToSwapMovies = true;
    }
    
    if (ofInRange(current()->getPosition(), 0.99, 1)) {
        if (_needToSwapMovies) {
            changePlayer();
        }
    }
    
    if (current()->isLoaded() && _isPlaying && !current()->isPlaying()) {
        start();
    }

    if (_isPlaying) {
        current()->play();
    } else {
        current()->stop();
    }

    
    current()->update();
}

void videoPlayers::start () {
    if (_isPlaying && current()->isPlaying()) {
        return;
    }

    // silently sync with global players state
    if (_isPlaying) {
        current()->play();
        current()->setPosition(0);
        return;
    }

    LOG_VP_NOTICE() << "start playing video: " << getCurrentMovieName();

    _isPlaying = true;
    current()->play();
    current()->setPosition(0);
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
    LOG_VP_NOTICE() << "changing video";
    current()->stop();
    //current()->setPosition(0);
    
    _curPlayerIndex= ofRandom(0, _players.size());
    _needToSwapMovies = false;

    LOG_VP_NOTICE() << "current video: " << getCurrentMovieName();
}


void videoPlayers::addVideoPlayer (const string& path) {
    LOG_VP_NOTICE() << "add video: " + path;

    ofVideoPlayer* result = new ofVideoPlayer();
    result->loadAsync(path);
    result->setLoopState(OF_LOOP_NONE);
    // mute
    result->setVolume(0);
    
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
    changePlayer();

    return true;
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

