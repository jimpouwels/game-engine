#ifndef vector2D_hpp
#define vector2D_hpp

namespace jimp {

struct Vector2D {
    float x;
    float y;
    
    void reset() {
        x = 0.0F;
        y = 0.0F;
    }
    
    Vector2D operator+(Vector2D other) {
        return Vector2D { .x = x + other.x, .y = y + other.y };
    }
    
    Vector2D operator-(Vector2D other) {
        return Vector2D { .x = x - other.x, .y = y - other.y };
    }
    
    bool operator<(Vector2D other) {
        return (x + y) < (other.x + other.y);
    }
    
    bool operator>(Vector2D other) {
        return (x + y) > (other.x + other.y);
    }
    
    static Vector2D empty() {
        return Vector2D { .x = 0, .y = 0 };
    }
    
    static Vector2D from(float x, float y) {
        return Vector2D { .x = x, .y = y };
    }
};

}

#endif
