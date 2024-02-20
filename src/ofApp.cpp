#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofLogNotice() << "setup: start";

    // prepare to render in framebuffer
	_fbo.allocate(GRAB_WIDTH, GRAB_HEIGHT, GL_RGBA);

    ofSetWindowTitle("CoCkeyer");
    ofBackground(0,0,0);
    ofSetFrameRate(30);
    ofSetFullscreen(true);
    ofSetWindowShape(GRAB_WIDTH, GRAB_HEIGHT);
    
    ofSetVerticalSync(true);

    try {
        initGui();
        initGrabber();
        _cocRecorder.init("out.mp4", {GRAB_WIDTH, GRAB_HEIGHT});

        _videoPlayers.initPlayers("bkgs");
        _greenscreen.setPixels(_grabber.getPixelsRef());    

        ofLogNotice() << "setup: done";        
    } catch (std::runtime_error& e) {
        ofLogError() << "setup failed: " << e.what();
        ofExit(1);
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    updateGui();

    _grabber.update();  
    
    if(_grabber.isFrameNew()) {
        _greenscreen.setPixels(_grabber.getPixelsRef());
    }

    _videoPlayers.current()->update();

    // ofLogNotice() << "test toggle val: " << _testToggleVal;
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255);

    // draw into the fbo
	_fbo.begin();
    {
        _videoPlayers.current()->draw(getLeftTopX(), getLeftTopY());

        _greenscreen.draw(getLeftTopX(), getLeftTopY(), GRAB_WIDTH, GRAB_HEIGHT, false);
    }
    _fbo.end();

    _cocRecorder.addFrame(_fbo);

    _fbo.draw(0, 0);


    if (_guiVisible) {
        _gui.draw();
    }
}

//--------------------------------------------------------------
void ofApp::exit(){
    _videoPlayers.free();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
   if (key == 'g') {
    _guiVisible = !_guiVisible;
   }

   if (key == 'm') {
        if (_mouseVisible) {
            ofGetWindowPtr()->hideCursor();
        }
        else {
            ofGetWindowPtr()->showCursor();
        }
        
        _mouseVisible = !_mouseVisible;
    }


}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    // exit if gui is not visible
    // FIXME: enable this logic
    // if (!_gui->isVisible()) {
    //     return;
    // }
    
    // ok. lets do it
    if(key == ' ') {
        _greenscreen.learnBgColor(_grabber.getPixelsRef());
    }

    if (key == 'p') {
        if (_videoPlayers.current()->isPlaying()) {
            _videoPlayers.current()->stop();
        } 
        else {
            _videoPlayers.current()->play();
        }
        
    }

    if (key == 'r') {
        _cocRecorder.toggleRecording();
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
   // exit if gui is not visible
    // if (!_gui->isVisible()) {
    //     return;
    // }
    
    // if (!_bkgColorPickerEnabled) {
    //     return;
    // }
    
    // debug
//    cout << "mouse dragged. dragStart.x=" << ofToString(dragStart.x) << " dragStart.y=" << ofToString(dragStart.y) << endl;
    
    // ok. lets do it
    if(x == _dragStart.x || y == _dragStart.y) {
        return;
    }
    
    
    _greenscreen.learnBgColor(_grabber.getPixelsRef(), _dragStart.x, _dragStart.y, x - _dragStart.x, y - _dragStart.y);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    _dragStart.set(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}

// 
// methods
// 

void ofApp::initGrabber () {
    //we can now get back a list of devices.
    vector<ofVideoDevice> devices = _grabber.listDevices();
    
    //
    // show info and
    // set DV-VCR as video input device
    //
    bool found = false;
    
    ofLogNotice() << "Webcams:";

    for(int i = 0; i < devices.size(); i++){
        ofLogNotice() << devices[i].id  << ": '" << devices[i].deviceName << "'";
        if( devices[i].bAvailable ){
            // set device to GoPro
            if (string("'GoPro'").compare(devices[i].deviceName) == 0) {
                ofLogNotice() << "setting webcam to Gopro";
                _grabber.setDeviceID(i);
                found = true;
            }
        } else {
            ofLogNotice() << devices[i].deviceName << " is unavailable";
        }
        
        // debug formats
//        vector<ofVideoFormat> formats = devices[i].formats;
//        for (auto j : formats) {
//            cout << "\twidth: " << j.width << " height: " << j.height << endl;
//        }
        
//        grabber.setDesiredFrameRate(25);
    }
    
    if (!found) {
        ofLogWarning() << "GoPro was not found. Fallback to device ID=0";
        _grabber.setDeviceID(0);
    }
    
    _grabber.initGrabber(GRAB_WIDTH, GRAB_HEIGHT);
}

int ofApp::getLeftTopX () {
    return (ofGetWidth() - GRAB_WIDTH) / 2;

}

// GUI
void ofApp::initGui () {
    _gui.setup(); // most of the time you don't need a name     

    _gui.add(_detailMask.setup("detail mask", _greenscreen.doDetailMask));
    _gui.add(_baseMask.setup("base mask", _greenscreen.doBaseMask));
    _gui.add(_chromaMask.setup("chroma mask", _greenscreen.doChromaMask));
    _gui.add(_greenSpill.setup("greenspill", _greenscreen.doGreenSpill));

    _gui.add(_baseMaskStrength.setup("base mask strength", _greenscreen.strengthBaseMask, 0.0, 1.f));
    _gui.add(_chromaMaskStrength.setup("chroma mask strength", _greenscreen.strengthChromaMask, 0.0, 1.f));
    _gui.add(_greenSpillStrength.setup("green spill strength", _greenscreen.strengthGreenSpill, 0.0, 1.f));
    
    _gui.add(_baseMaskBlack.setup("base mask black", _greenscreen.clipBlackBaseMask, 0.0, 1.f));
    _gui.add(_baseMaskWhite.setup("base mask white", _greenscreen.clipWhiteBaseMask, 0.0, 1.f));
    _gui.add(_detailMaskBlack.setup("detail mask black", _greenscreen.clipBlackDetailMask, 0.0, 1.f));
    _gui.add(_detailMaskWhite.setup("detail mask white", _greenscreen.clipWhiteDetailMask, 0.0, 1.f));
    _gui.add(_endMaskBlack.setup("end mask black", _greenscreen.clipBlackEndMask, 0.0, 1.f));
    _gui.add(_endMaskWhite.setup("end mask white", _greenscreen.clipWhiteEndMask, 0.0, 1.f));
        
    

    // _gui->addToggle("Enable bkg color picker", &_bkgColorPickerEnabled);
    // _gui->addLabel("bkg color");
    // _gui->addFPS();
    // _gui.add(_testToggle.setup("test toggle", &_testToggleVal));

    _guiVisible = false;
}

void ofApp::updateGui () {
    _greenscreen.doDetailMask = _detailMask;
    _greenscreen.doBaseMask = _baseMask;
    _greenscreen.doChromaMask = _chromaMask;
    _greenscreen.doGreenSpill = _greenSpill;

    _greenscreen.strengthBaseMask = _baseMaskStrength;
    _greenscreen.strengthChromaMask = _chromaMaskStrength;
    _greenscreen.strengthGreenSpill = _greenSpillStrength;

    _greenscreen.clipBlackBaseMask = _baseMaskBlack;
    _greenscreen.clipWhiteBaseMask = _baseMaskWhite;
    _greenscreen.clipBlackDetailMask = _detailMaskBlack;
    _greenscreen.clipWhiteDetailMask = _detailMaskWhite;
    _greenscreen.clipBlackEndMask = _endMaskBlack;
    _greenscreen.clipWhiteEndMask = _endMaskWhite;
}

// coordinates helpers

int ofApp::getLeftTopY () {
    return (ofGetHeight() - GRAB_HEIGHT) / 2;
}


int ofApp::getBottomRightX () {
    return getLeftTopX() + GRAB_WIDTH;
}

int ofApp::getBottomRightY () {
    return getLeftTopY() + GRAB_HEIGHT;
}