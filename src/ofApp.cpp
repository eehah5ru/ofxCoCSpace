#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofLogNotice() << "setup: start";

    // prepare to render in framebuffer
    _fbo.allocate(GRAB_WIDTH, GRAB_HEIGHT, GL_RGBA);
    _fboCam.allocate(GRAB_WIDTH, GRAB_HEIGHT, GL_RGBA);
    _fboSubs.allocate(GRAB_WIDTH, GRAB_HEIGHT, GL_RGBA);
    
    ofSetWindowTitle("CoCkeyer");
    ofBackground(0,0,0);
    ofSetFrameRate(30);
    
    //ofSetWindowShape(GRAB_WIDTH, GRAB_HEIGHT);
    
    ofSetVerticalSync(true);

    try {
        initGui();
        _camera.get()->init();

        //
        // mp4
        // 
        _cocRecorderCam.init("recordings", "cam.mp4", {GRAB_WIDTH, GRAB_HEIGHT});
        _cocRecorderAll.init("recordings", "all.mp4", {GRAB_WIDTH, GRAB_HEIGHT});
        //
        // raw
        // 
        // _cocRecorderCam.init("cam.raw", {GRAB_WIDTH, GRAB_HEIGHT});
        // _cocRecorderAll.init("all.raw", {GRAB_WIDTH, GRAB_HEIGHT});


        _videoPlayers.initPlayers("bkgs");
        _subs.init("subs");
        
        _greenscreen.setPixels(_camera.get()->getPixelsRef());

        ofLogNotice() << "setup: done";        
    } catch (std::runtime_error& e) {
        ofLogError() << "setup failed: " << e.what();
        ofExit(1);
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    updateGui();
    updateGrabber();

    _videoPlayers.update();
    _subs.update();
    
    _status.update();
    _subsStatus.update();


    //ofLogNotice() << "width: " << ofGetWidth();
    //ofLogNotice() << "height: " << ofGetHeight();

    // ofLogNotice() << "test toggle val: " << _testToggleVal;
}

//--------------------------------------------------------------
void ofApp::draw(){
    // clear the screen;
    ofSetColor(0,0,0);
    ofFill();

    ofSetColor(255);

    _fboCam.begin();
    { 
        if (_cameraEnabled) {
            // relative to fbo!!       
          _camera.get()->draw(0, 0);
        }
    }
    _fboCam.end();
   
    _cocRecorderCam.addFrame(_fboCam);

    // draw everything into the fbo
    _fbo.begin();
    {
        // drawing relative to fbo
        _videoPlayers.current()->draw(0, 0);

        if (_cameraEnabled) {
            _greenscreen.draw(0, 0, GRAB_WIDTH, GRAB_HEIGHT, false);
        }

        // relaltive to fbo
        _subs.draw(0, 0);
        

        
    }
    _fbo.end();

    _cocRecorderAll.addFrame(_fbo);

    _fbo.draw(getLeftTopX(), getLeftTopY());

    ofPushStyle();

    ofSetColor( _cocRecorderCam.getColoredStatus());
    ofDrawCircle( getLeftTopX() + 15, getLeftTopY() + 15, 10 );

    ofSetColor( _cocRecorderAll.getColoredStatus());
    ofDrawCircle( getLeftTopX() + 40, getLeftTopY() + 15, 10 );

    ofPopStyle();

    // video players status
    _status.draw(getLeftTopX() + 65, getLeftTopY() + 15); 

    // subs status
    _status.draw(getLeftTopX() + 90, getLeftTopY() + 15); 


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

    if (key == 'f') {
        ofToggleFullscreen();
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
      _greenscreen.learnBgColor(_camera.get()->getPixelsRef());
    }

    if (key == 'r') {
        _cocRecorderAll.toggleRecording();
        _cocRecorderCam.toggleRecording();
    }

    //
    // video players ctrls
    // 
    if (key == 'p') {
        _videoPlayers.togglePlay();        
    }
    
    if (key == 'j') {
        _status.compose(_videoPlayers.goToPrevFolder());
    }

    if (key == 'k') {
        _status.compose(_videoPlayers.goToNextFolder());
    }

    if (key == 'h') {
        _status.compose(_videoPlayers.goToPrevVideo());
    }

    if (key == 'l') {
        _status.compose(_videoPlayers.goToNextVideo());
    }

    if (key == 'n') {
        _videoPlayers.changePlayer();
    }

    //
    // subs ctrls
    // the same as for video but in caps
    // 
    if (key == 'P') {
      _subs.getPlayers().togglePlay();        
    }
    
    if (key == 'J') {
      _subsStatus.compose(_subs.getPlayers().goToPrevFolder());
    }

    if (key == 'K') {
        _subsStatus.compose(_subs.getPlayers().goToNextFolder());
    }

    if (key == 'H') {
        _subsStatus.compose(_subs.getPlayers().goToPrevVideo());
    }

    if (key == 'L') {
        _subsStatus.compose(_subs.getPlayers().goToNextVideo());
    }

    if (key == 'N') {
        _subs.getPlayers().changePlayer();
    }

    //
    // end subs ctrls
    // 
    

    //
    // CAMERA CTRLS
    // 

    // toggle camera grabber
    if (key == 'c') {
        _cameraEnabled = !_cameraEnabled;
        LOG_APP_NOTICE() << "toggled camera " << _cameraEnabled;
    }

    //
    // reinit camera with device/stream num
    // 
    vector<char> keys = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};

    if (find(keys.begin(), keys.end(), key) != keys.end()) {
      int keyIndex = key - '0';
      _camera.get()->reinit(keyIndex);
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
        
    _greenscreen.learnBgColor(_camera.get()->getPixelsRef(), _dragStart.x, _dragStart.y, x - _dragStart.x, y - _dragStart.y);
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


void ofApp::updateGrabber () {
    
    if (!_cameraEnabled) {
        return;
    }

    _camera.get()->update();  

    
    if(_camera.get()->isFrameNew()) {
      _greenscreen.setPixels(_camera.get()->getPixelsRef());
    }
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
