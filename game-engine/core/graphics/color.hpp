#ifndef color_hpp
#define color_hpp

#include <sstream>

namespace jimp {

struct Color {
    float r;
    float g;
    float b;
    
    void reset() {
        r = 0;
        g = 0;
        b = 0;
    }
    
    static Color black() {
        return Color { .r = 0, .g = 0, .b = 0 };
    }
    
    static Color full() {
        return Color { .r = 255, .g = 255, .b = 255 };
    }
    
    static Color from(float r, float g, float b) {
        return Color { .r = r, .g = g, .b = b };
    }
    
    static Color from(std::string hexString) {
        unsigned int x;
        std::stringstream ss;
        ss << std::hex << hexString.substr(1);
        ss >> x;
        return from(x >> 16 & 0xff, x >> 8 & 0xff, x & 0xff);
    }
};

}

#endif
