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
    cerr << "searching for videos in " << path << "\n";
    ofDirectory dir;
    dir.open(path);
    dir.allowExt("mov");
    dir.allowExt("mp4");
    
    dir.listDir();

    if (dir.size() == 0) {
        ofLogError() << "video data directory is empty!";
        
        throw std::runtime_error("video data directory is empty!");
    }

    for (auto i: dir.getFiles()) {
        addVideoPlayer(i.getAbsolutePath());
    }
    
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


void videoPlayers::changePlayer() {
    current()->stop();
    
    _curPlayerIndex= ofRandom(0, _players.size() - 1);
}


void videoPlayers::addVideoPlayer (const string& path) {
    ofLogNotice() << "add video: " + path;
    ofVideoPlayer* result = new ofVideoPlayer();
    result->loadAsync(path);
    result->setLoopState(OF_LOOP_NONE);
    // mute
    result->setVolume(0);
    
    _players.push_back(result);
}
