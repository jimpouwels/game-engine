#ifndef timing_hpp
#define timing_hpp

namespace jimp {

class Timing {
    
public:
    static float toValueForElapsedTime(float valuePerSecond, float elapsedTime) {
        return valuePerSecond / (1.0F / elapsedTime);
    }
    
    static Vector2D toValueForElapsedTime(Vector2D vector2D, float elapsedTime) {
        vector2D.x = toValueForElapsedTime(vector2D.x, elapsedTime);
        vector2D.y = toValueForElapsedTime(vector2D.y, elapsedTime);
        return vector2D;
    }
    
};

}

#endif
