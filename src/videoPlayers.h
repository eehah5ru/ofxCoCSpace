#pragma once

//
//  videoPlayers.h
//  deSerialKeyer
//
//  Created by nicola spesivcev on 08.11.14.
//
//

#include "ofMain.h"

// Logging macros
#define LOG_VP_ERROR() ofLogError( "videoPlayers" ) << __FUNCTION__ << ": "
#define LOG_VP_WARNING() ofLogWarning( "videoPlayers" ) << __FUNCTION__ << ": "
#define LOG_VP_NOTICE() ofLogNotice( "videoPlayers" ) << __FUNCTION__ << ": "
#define LOG_VP_VERBOSE() ofLogVerbose( "videoPlayers" ) << __FUNCTION__ << ": "
#define LOG_VP() LOG_VP_NOTICE()


#ifndef deSerialKeyer_videoPlayers_h
#define deSerialKeyer_videoPlayers_h

class videoPlayers {
    vector<ofVideoPlayer *> _players;
    
    int _curPlayerIndex = 0;
    
    bool _isPlaying = false;

public:
    videoPlayers ();
    ~videoPlayers ();
    
    void initPlayers (string path);
    
    void free ();
    
    ofVideoPlayer* current ();



    string getCurrentMovieName ();
    
    bool changePlayer ();

    bool goToPrevVideo();
    bool goToNextVideo();
    bool goToNextFolder();
    bool goToPrevFolder();

    void togglePlay ();
    void stop ();
    void start ();
    
    // interface to ofApp
    void update ();
    
    //void draw (int x, int y, int w, int h);
    
private:
    
    bool _needToSwapMovies = false;

    int _currentDirIndex = 0;

    bool _initialized = false;

    std::string _basePath = "bkgs";

    void addVideoPlayer(const string& path);
    
    bool goToFolder(int dirIndex);

    bool goToVideo(int newPlayerIndex);

    bool hasFolder(int dirIndex);

    bool hasVideo(int videIndex);

    string mkFolderPath(int dirIndex);    
};

#endif
