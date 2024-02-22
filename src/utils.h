#pragma once

#include "ofMain.h"

class Status {
    public:

    enum Value {OK, ERROR, NEUTRAL};

    Status(Value);
    ~Status() = default;

    static Status ok();
    static Status error();
    static Status neutral();

    ofColor toColor();

    void reset();

    void changed();

    void compose (bool v);

    void draw(int x, int y);

    void update();

    private:

    Value _v = NEUTRAL;

    int _changed = 0;
};