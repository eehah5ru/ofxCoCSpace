#pragma once

#include <string>

#include "ofMain.h"
#include "ofxFFmpeg.h"

// Logging macros
#define LOG_COCR_ERROR() ofLogError( "cocRecorder" ) << __FUNCTION__ << ": "
#define LOG_COCR_WARNING() ofLogWarning( "cocRecorder" ) << __FUNCTION__ << ": "
#define LOG_COCR_NOTICE() ofLogNotice( "cocRecorder" ) << __FUNCTION__ << ": "
#define LOG_COCR_VERBOSE() ofLogVerbose( "cocRecorder" ) << __FUNCTION__ << ": "
#define LOG_COCR() LOG_COCR_NOTICE()

class cocRecorder {

    public:

    cocRecorder();
    ~cocRecorder();

    void init();
  void init (string path, string baseName, glm::ivec2 videoResolution);

    void addFrame(ofFbo &fbo);

    void toggleRecording();

    void start ();
    void stop ();

    bool isRecording();

    ofColor getColoredStatus ();

    private:

  std::string _path = "recordings";
    std::string _baseName = "output.mp4";

    ofxFFmpeg::Recorder _recorder;
    ofxFFmpeg::RecorderSettings _recorderSettings;
    
    //ofPixels mPix;
};
