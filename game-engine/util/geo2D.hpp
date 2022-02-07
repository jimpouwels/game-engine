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
        float invertedAngle = angle;
        if (angle == 0 || angle == 180) {
            invertedAngle += 180;
        } else if (angle < 90 || (angle < 270 && angle > 180)) {
            invertedAngle = 180 - angle;
        } else {
            invertedAngle = 180 + (360 - angle);
        }
        return normalizeAngle(invertedAngle);
    }
    
    static float inverseAngleHorizontally(float angle) {
        float invertedAngle = angle;
        if (invertedAngle == 0 || angle == 180) {
            invertedAngle += 180;
        } if (angle < 90 || angle > 270) {
            invertedAngle = 360 - angle;
        } else {
            invertedAngle = 180 - (angle - 180);
        }
        return normalizeAngle(invertedAngle);
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
