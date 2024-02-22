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
    
}


void videoPlayers::initPlayers(string path) {
    LOG_VP_NOTICE() << "searching for videos in " << path << "\n";
    ofDirectory dir;
    dir.open(path);
    dir.allowExt("mov");
    dir.allowExt("mp4");
    
    dir.listDir();

    if (dir.size() == 0) {
        LOG_VP_ERROR() << "video data directory is empty!";
        
        throw std::runtime_error("video data directory is empty!");
    }

    for (auto i: dir.getFiles()) {
        addVideoPlayer(i.getAbsolutePath());
    }

    LOG_VP_NOTICE() << "videos to play: " << _players.size();
    
    changePlayer();

}


void videoPlayers::free () {
    for (auto i: _players) {
        delete i;
    }
}

ofVideoPlayer* videoPlayers::current() {
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
