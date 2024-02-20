#include "cocRecorder.h"

#include <chrono>
#include <ctime>
#include <iomanip>

cocRecorder::cocRecorder () {

}

cocRecorder::~cocRecorder () {

}

void cocRecorder:: init () {
    init("output.mp4", {640, 480});
}

void cocRecorder::init (string baseName, glm::ivec2 videoResolution) {
    LOG_COCR() << "initializing";

    _baseName = baseName;

    _recorderSettings.outputPath = ofGetTimestampString() + "_" + _baseName;
    _recorderSettings.videoResolution = videoResolution;
}


void cocRecorder::addFrame (ofFbo &fbo) {
    ofPixels pix;

    if ( _recorder.isRecording() ) {

		fbo.readToPixels( pix );  // use ofxFastFboReader to speed up fbo -> pixels read
		if ( pix.getWidth() > 0 && pix.getHeight() > 0 ) {
		    _recorder.addFrame( pix );	// call this as often as possible, the recorder will accept the pixels when its ready for a new frame
		}
        
	}
}

void cocRecorder::start () {
    if (_recorder.isRecording()) {
        LOG_COCR_WARNING() << "already started!";
        return;
    }
    LOG_COCR() << "start";
    // name of the file with actual timestamp  
    _recorderSettings.outputPath = ofGetTimestampString() + "_" + _baseName;
    
    _recorder.start(_recorderSettings);
 
}

void cocRecorder::stop () {
    if (!_recorder.isRecording()) {
        LOG_COCR_WARNING() << "already stopped";
        return;
    }
    LOG_COCR() << "stop";
    _recorder.stop();
}


void cocRecorder::toggleRecording() {
    LOG_COCR() << "toggleRecording";

    if (_recorder.isRecording()) {
        stop();
    } else {
        start();
    }
}

bool cocRecorder::isRecording () {
    return _recorder.isRecording();
}


ofColor cocRecorder::getColoredStatus () {
    if (_recorder.isRecording()) {
        return ofColor::red;
    }

    if (_recorder.isReady()) {
        return ofColor::green;
    }

    // if not recording, but not ready, it's still processing a previous recording
    return ofColor::yellow;
}