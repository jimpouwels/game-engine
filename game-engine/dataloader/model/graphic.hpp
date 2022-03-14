#ifndef graphic_hpp
#define graphic_hpp

#include "vector2D.hpp"

namespace jimp {

struct Graphic {
    std::string description;
    std::string type;
    Vector2D position = Vector2D::empty();
    float scale = 1.0f;
    bool applyGravity;
    float rotationAngle = 0.0f;
};

}

#endif
