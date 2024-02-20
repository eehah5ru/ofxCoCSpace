#pragma once

#include "ofMain.h"
#include "ofxGreenscreen.h"
#include "ofxGui.h"

#include "videoPlayers.h"
#include "cocRecorder.h"


#define GRAB_WIDTH  1920
#define GRAB_HEIGHT 1080

class ofApp : public ofBaseApp{

public:
    void setup() override;
    void update() override;
    void draw() override;
    void exit() override;

    void keyPressed(int key) override;
    void keyReleased(int key) override;
    void mouseMoved(int x, int y ) override;
    void mouseDragged(int x, int y, int button) override;
    void mousePressed(int x, int y, int button) override;
    void mouseReleased(int x, int y, int button) override;
    void mouseScrolled(int x, int y, float scrollX, float scrollY) override;
    void mouseEntered(int x, int y) override;
    void mouseExited(int x, int y) override;
    void windowResized(int w, int h) override;
    void dragEvent(ofDragInfo dragInfo) override;
    void gotMessage(ofMessage msg) override;

    private:
// attrs
    ofxGreenscreen      _greenscreen;
    ofVideoGrabber      _grabber;
    
    videoPlayers        _videoPlayers;

    cocRecorder         _cocRecorder;

    // gonna render to framebuffer;
    ofFbo               _fbo;
    
    bool                _needToSwapMovies;    
    bool                _bkgColorPickerEnabled;
    bool                _mouseVisible;
    bool _guiVisible;

    ofPoint _dragStart;
    ofPoint _greenPixelPos;

    // GUI related
    ofxPanel _gui;

    ofxToggle _detailMask;
    ofxToggle _baseMask;
    ofxToggle _chromaMask;
    ofxToggle _greenSpill;

    ofxFloatSlider _baseMaskStrength;
    ofxFloatSlider _chromaMaskStrength;
    ofxFloatSlider _greenSpillStrength;

    ofxFloatSlider _baseMaskBlack;
    ofxFloatSlider _baseMaskWhite;
    ofxFloatSlider _detailMaskBlack;
    ofxFloatSlider _detailMaskWhite;
    ofxFloatSlider _endMaskBlack;
    ofxFloatSlider _endMaskWhite;
    

    bool _testToggleVal;
    ofxToggle _testToggle;
   

// methods
  // init grabber
    void initGrabber ();
    
    void initGui ();

    void updateGui ();
    
    // void updateBgColorFormGuiBgColor ();
    
    // void updateGuiBgColorFormBgColor ();
    
    int getLeftTopX ();
    
    int getLeftTopY ();
    
    int getBottomRightX ();
    
    int getBottomRightY ();
};
