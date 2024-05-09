#pragma once

#include "ofMain.h"
#include "videoPlayers.h"
#include "videoPlayers.h"

// Logging macros
#define LOG_SUBS_ERROR() ofLogError( "Subs" ) << __FUNCTION__ << ": "
#define LOG_SUBS_WARNING() ofLogWarning( "Subs" ) << __FUNCTION__ << ": "
#define LOG_SUBS_NOTICE() ofLogNotice( "Subs" ) << __FUNCTION__ << ": "
#define LOG_SUBS_VERBOSE() ofLogVerbose( "Subs" ) << __FUNCTION__ << ": "
#define LOG_SUBS() LOG_SUBS_NOTICE()

class Subs {  
 public:
  Subs ();
  ~Subs ();

  void init (string path);

  void start ();
  void stop ();

  void update ();

  videoPlayers& getPlayers ();

  void draw (int x, int y);

 private:
  videoPlayers _players;
  
  ofTexture _texture;
  ofShader _shader;
  
};
