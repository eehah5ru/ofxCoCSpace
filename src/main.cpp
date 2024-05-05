#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){

    //Use ofGLFWWindowSettings for more options like multi-monitor fullscreen
    ofEnableGLDebugLog();
    ofGLFWWindowSettings settings;
    //settings.setSize(GRAB_WIDTH, GRAB_HEIGHT);
    //settings.numSamples = 0; // for RPI performance
    settings.windowMode =  OF_FULLSCREEN; // OF_WINDOW; //can also be OF_FULLSCREEN
    //settings.windowMode =  OF_WINDOW;
  //  settings.width = GRAB_WIDTH;

    auto window = ofCreateWindow(settings);

    ofRunApp(window, make_shared<ofApp>());
    ofRunMainLoop();

}
