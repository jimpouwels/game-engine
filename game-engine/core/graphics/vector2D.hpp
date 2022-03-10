#ifndef vector2D_hpp
#define vector2D_hpp

namespace jimp {

struct Vector2D {
    float x;
    float y;
    
    void reset() {
        x = 0;
        y = 0;
    }
    
    Vector2D operator+(Vector2D other) {
        return Vector2D { .x = x + other.x, .y = y + other.y };
    }
    
    Vector2D operator-(Vector2D other) {
        return Vector2D { .x = x - other.x, .y = y - other.y };
    }
    
    static Vector2D empty() {
        return Vector2D { .x = 0, .y = 0 };
    }
};

}

#endif
