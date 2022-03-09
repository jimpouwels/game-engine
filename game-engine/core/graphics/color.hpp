#ifndef color_hpp
#define color_hpp

namespace jimp {

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    
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
    
    static Color custom(uint8_t r, uint8_t g, uint8_t b) {
        return Color { .r = r, .g = g, .b = b };
    }
};

}

#endif
