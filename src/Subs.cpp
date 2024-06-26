#include "Subs.h"
#include "ofApp.h"
#include "ofGraphicsBaseTypes.h"
#include "ofPixels.h"
#include "videoPlayers.h"
#include <stdexcept>

Subs::Subs () {
}

Subs::~Subs () {
  _players.free();
}

void Subs::init(string path) {
  LOG_SUBS() << "init with basePath: " << path;

  _players.initPlayers(path);

  if(ofIsGLProgrammableRenderer()){
		_shader.load("shadersGL3/subs_keyer");
	}else{
		// shader.load("shadersGL2/shader");
    throw std::runtime_error("Subs.init: shaders are v2");
	}

  // _texture.allocate(GRAB_WIDTH, GRAB_HEIGHT, GL_RGBA);

  LOG_SUBS() << "init done";
}

videoPlayers& Subs::getPlayers () {
  return _players;
}

void Subs::update () {
  _players.update();

  // if (!_players.current()->isFrameNew()) {
  //   return;
  // }

  // ofPixels px = _players.current()->getPixels();
  // ofPixels rgbaPx;

  // rgbaPx.allocate(_players.current()->getWidth(), _players.current()->getHeight(), OF_PIXELS_RGBA);

  // size_t i = 0;
  // size_t j = 0;
  // for (i=0, j=0; i < px.size() - px.getNumChannels(); j=j+rgbaPx.getNumChannels(), i=i+px.getNumChannels()) {
  //   ofColor c(px[i], px[i+1], px[i+2]);
  //   rgbaPx[j] = px[i];
  //   rgbaPx[j+1] = px[i+1];
  //   rgbaPx[j+2] = px[i+2];
  //   // alpha
  //   rgbaPx[j+3] = c.getLightness() < 10 ? 0 : 255;
  // }

  // _texture.loadData(rgbaPx);
  
}

void Subs::draw (int x, int y) {
  if (!_players.current()->isLoaded()) {
    return;
  }
  // _texture.draw(x, y);

  _shader.begin();

  _players.current()->draw(x, y);

  _shader.end();
}

void Subs::toggleVisibility() {
  _isVisible = !_isVisible;
  _players.current()->setPaused(_isVisible);  
}
