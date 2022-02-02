#ifndef geo2d_hpp
#define geo2d_hpp

#include <math.h>
#include "vector2D.hpp"

namespace jimp {

class Geo2D {

public:
    static Vector2D deltaVectorByAngle(float angle, float distance) {
        float deltaX = distance * sin(M_PI * 2 * angle / 360);
        float deltaY = -(distance * cos(M_PI * 2 * angle / 360));
        return Vector2D { .x = deltaX, .y = deltaY };
    }
    
    static float inverseAngleVertically(float angle) {
        float newAngle = angle;
        if (newAngle == 0 || newAngle == 180) {
            newAngle = newAngle + 180;
        } else if (newAngle < 90 || (newAngle < 270 && newAngle > 180)) {
            newAngle = newAngle + 90;
        } else {
            newAngle = newAngle - 90;
        }
        return normalizeAngle(newAngle);
    }
    
    static float inverseAngleHorizontally(float angle) {
        float newAngle = angle;
        if (newAngle == 0 || newAngle == 180) {
            newAngle = newAngle + 180;
        } else if (angle < 90 || (newAngle < 270 && newAngle > 180)) {
            newAngle = newAngle - 90;
        } else {
            newAngle = newAngle + 90;
        }
        return normalizeAngle(newAngle);
    }
    
    static float normalizeAngle(float angle) {
        if (angle >= 360) {
            return angle -= 360;
        } else if (angle <= 0) {
            return angle += 360;
        }
        return angle;
    }
    
};

}

#endif
