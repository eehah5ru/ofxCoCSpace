#include "ofMain.h"
#include "ofApp.h"

#include "ofGstVideoPlayer.h"
#include <gst/gst.h>
#include <gst/video/video.h>
#include <gst/app/gstappsink.h>
#include "ofConstants.h"
#include "ofGstUtils.h"

void setupGstPlugins () {
  GstRegistry *reg = gst_registry_get();
  GstPluginFeature *plugin;
  plugin = gst_registry_lookup_feature(reg, "mppvideodec");
  gst_plugin_feature_set_rank(plugin, GST_RANK_PRIMARY);

}

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

    settings.setGLVersion(3,2);
    
    auto window = ofCreateWindow(settings);

    setupGstPlugins();

    ofRunApp(window, make_shared<ofApp>());
    ofRunMainLoop();
}
