#include "Camera.h"
#include "ofApp.h"
#include "utils.h"
#include "ofGraphicsBaseTypes.h"
#include "ofUtils.h"
#include "ofVideoBaseTypes.h"
#include <cstdio>
#include <cstring>
// #include <format>
#include <memory>
#include <stdexcept>
// #include <cstring>

//
//
// CAMERA
//
//

shared_ptr<Camera> Camera::createCamera() {
  const string cam_type = ofGetEnv("CAMERA");

  if (cam_type.compare("LOCAL") ==  0) {
    return make_shared<LocalCamera>();
  }
  if (cam_type.compare("REMOTE") == 0) {
    return make_shared<RemoteCamera>();
  }

  throw std::runtime_error("Unknown CAMRERA env value.");
}



//
//
// LOCAL CAMERA
//
// 
LocalCamera::LocalCamera() {
}

LocalCamera::~LocalCamera() {
}

// ofBaseVideo& LocalCamera::getGrabber () {
//   return _grabber; 
// }

void LocalCamera::init () {
  //we can now get back a list of devices.
  vector<ofVideoDevice> devices = _grabber.listDevices();
    
  //
  // show info and
  // set DV-VCR as video input device
  //
  bool found = false;
    
  LOG_CAM_NOTICE() << "Webcams:";

  for(int i = 0; i < devices.size(); i++){
    LOG_CAM_NOTICE() << devices[i].id  << ": '" << devices[i].deviceName << "'";
    if( devices[i].bAvailable ){
      // set device to GoPro
      if (string("'GoPro'").compare(devices[i].deviceName) == 0) {
        LOG_CAM_NOTICE() << "setting webcam to Gopro";
        _grabber.setDeviceID(i);
        found = true;
      }
    } else {
      LOG_CAM_NOTICE() << devices[i].deviceName << " is unavailable";
    }
        
    // debug formats
    //        vector<ofVideoFormat> formats = devices[i].formats;
    //        for (auto j : formats) {
    //            cout << "\twidth: " << j.width << " height: " << j.height << endl;
    //        }
        
    //        grabber.setDesiredFrameRate(25);
  }
    
  if (!found) {
    LOG_CAM_WARNING() << "GoPro was not found. Fallback to device ID=0";
    _grabber.setDeviceID(0);
  }
    
  _grabber.initGrabber(GRAB_WIDTH, GRAB_HEIGHT);
}

void LocalCamera::reinit(int index) {
  throw std::runtime_error("LocalCamera::reinit - unimplemented");
}

void LocalCamera::update() {
  _grabber.update();
}

void LocalCamera::draw(int x, int y) {
  _grabber.draw(x, y);
}

ofPixelsRef LocalCamera::getPixelsRef() {
  return _grabber.getPixelsRef();
}

bool LocalCamera::isFrameNew() {
  return _grabber.isFrameNew();
}

//
//
// REMOTE CAMERA
//
//
RemoteCamera::RemoteCamera() {
}

RemoteCamera::~RemoteCamera() {
}

// ofBaseVideo& RemoteCamera::getGrabber () {
//   return _grabber; 
// }

void RemoteCamera::init() {
  // _grabber.load("rtmp://192.168.0.115:1935/live/1");
  // _grabber.load("");

  // _grabber.play();

  // starting from stream 1
  reinit(1);
}

void RemoteCamera::reinit (int index) {
  string host = "192.168.3.4:1935";
  string url = string_format("rtmp://%s/live/%d", host, index);

  
  _grabber.stop();
  _grabber.close();
  _grabber.load(url);
  _grabber.play();
}

void RemoteCamera::update() {
  _grabber.update();
}

void RemoteCamera::draw(int x, int y) {
  _grabber.draw(x, y);
}

ofPixelsRef RemoteCamera::getPixelsRef() {
  return _grabber.getPixelsRef();
}

bool RemoteCamera::isFrameNew() {
  return _grabber.isFrameNew();
}

