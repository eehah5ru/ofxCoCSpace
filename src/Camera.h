#pragma once

#include "ofGraphicsBaseTypes.h"
#include "ofMain.h"
#include "ofVideoBaseTypes.h"
#include "ofVideoGrabber.h"

// Logging macros
#define LOG_CAM_ERROR() ofLogError( "camera" ) << __FUNCTION__ << ": "
#define LOG_CAM_WARNING() ofLogWarning( "camera" ) << __FUNCTION__ << ": "
#define LOG_CAM_NOTICE() ofLogNotice( "camera" ) << __FUNCTION__ << ": "
#define LOG_CAM_VERBOSE() ofLogVerbose( "camera" ) << __FUNCTION__ << ": "
#define LOG_CAM() LOG_CAM_NOTICE()

//
// Camera wrapper for LOCAL and remote ones
// 
class Camera {
public:
  static shared_ptr<Camera> createCamera();

  virtual ~Camera () = default;
  
  virtual void init () = 0;

  virtual void reinit (int index) = 0;

  virtual void update () = 0;

  virtual void draw (int x, int y) = 0;

  virtual ofPixelsRef getPixelsRef () = 0;

  virtual bool isFrameNew () = 0;

//   void toggleCamera();

//   bool isCameraEnabled ();
  
private:
//   bool _cameraEnabled = true;

  // virtual ofBaseVideo& getGrabber() = 0;
  
};


//
// local camera
//
class LocalCamera:
  public Camera {

public:
  LocalCamera ();
  ~LocalCamera ();
  
  void init () override;
  void reinit (int index) override;
  void update () override;
  void draw (int x, int y) override;
  ofPixelsRef getPixelsRef () override;
  bool isFrameNew() override;

private:
  ofVideoGrabber _grabber;

// protected:
//   ofBaseVideo& getGrabber () override;
};

//
// remote camera
//
class RemoteCamera:
  public Camera {

public:
  RemoteCamera ();
  ~RemoteCamera ();

  void init () override;
  void reinit (int index) override;
  
  void update () override;
  void draw (int x, int y) override;
  ofPixelsRef getPixelsRef () override;
  bool isFrameNew() override;
private:
  ofVideoPlayer       _grabber;

// protected:
//   ofBaseVideo& getGrabber () override;
  
};
