#ifndef graphic_hpp
#define graphic_hpp

#include "vector2D.hpp"
#include "color.hpp"

namespace jimp {

struct Graphic {
    std::string description;
    std::string type;
    Vector2D position = Vector2D::empty();
    float scale = 1.0f;
    bool applyGravity = false;
    bool applyScrolling = false;
    float rotationAngle = 0.0f;
    int rows = 0;
    int cols = 0;
    bool isMainCharacter = false;
    int zIndex = 0;
    bool collidable = false;
    Color rgb = Color::full();
    bool animateRgb = false;
    Color animateRgbFrom = Color::full();
    Color animateRgbTo = Color::full();
    float animateRgbSeconds = 0.0f;
};

}

#endif
