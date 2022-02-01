#ifndef geo2d_hpp
#define geo2d_hpp

#include <math.h>
#include "point2D.hpp"

namespace jimp {

class Geo2D {

public:
    static Point2D deltaByAngle(float angle, float distance) {
        float deltaX = distance * sin(M_PI * 2 * angle / 360);
        float deltaY = -(distance * cos(M_PI * 2 * angle / 360));
        return Point2D { .x = deltaX, .y = deltaY };
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
