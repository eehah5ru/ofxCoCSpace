#include "utils.h"


Status::Status(Value v) {
    _v = v;
}

Status Status::ok () {
    return Status(OK);
}

Status Status::error() {
    return Status(ERROR);
}

Status Status::neutral() {
    return Status(NEUTRAL);
}

ofColor Status::toColor() {
    if (_v == OK) {
        return ofColor::green;
    }

    if (_v == ERROR) {
        return ofColor::red;
    }

    return ofColor::blue;
}

void Status::reset() {
    _v = NEUTRAL;
}

void Status::compose(bool v) {
    if (v) {
        switch (_v) {
            case NEUTRAL:
                _v = OK;
                changed();
                break;
        }
    } 
    else {
        switch (_v) {
            case NEUTRAL:
                _v = ERROR;
                changed();
                break;
            case OK:
                _v = ERROR;
                changed();
                break;
        }
    }
}


void Status::update() {
    if (_changed == 0) {
        _v = NEUTRAL;
        return;
    }

    _changed--;
}

void Status::draw(int x, int y) {
    ofPushStyle();
    ofSetColor(toColor());
    ofFill();
    ofCircle(x, y, 10);
    ofPopStyle();
}


void Status::changed() {
    _changed = ofGetFrameRate() / 2;
}
