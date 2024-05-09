#pragma once

#include "ofMain.h"
#include <memory>
#include <string>
#include <stdexcept>


//
// status class
// 

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

//
// string formatter
//
template<typename ... Args>
std::string string_format( const std::string& format, Args ... args )
{
    int size_s = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
    auto size = static_cast<size_t>( size_s );
    std::unique_ptr<char[]> buf( new char[ size ] );
    std::snprintf( buf.get(), size, format.c_str(), args ... );
    return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}
