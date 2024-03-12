#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){

    //Use ofGLFWWindowSettings for more options like multi-monitor fullscreen
    ofGLWindowSettings settings;
    settings.setSize(GRAB_WIDTH, GRAB_HEIGHT);
    settings.windowMode =  OF_FULLSCREEN; // OF_WINDOW; //can also be OF_FULLSCREEN
  //  settings.width = GRAB_WIDTH;

    auto window = ofCreateWindow(settings);

    ofRunApp(window, make_shared<ofApp>());
    ofRunMainLoop();

}
