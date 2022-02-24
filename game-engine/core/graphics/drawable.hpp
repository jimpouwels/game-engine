#ifndef drawable_hpp
#define drawable_hpp

#include "vector2D.hpp"

namespace jimp {

class Drawable {
      
private:
    Vector2D position = { .x = 0, .y = 0};
    Vector2D rotationPoint = { .x = 0, .y = 0 };
    float scale = 1.0F;
    float angle = 0.0F;
        
public:
    Drawable(float x, float y, float scale, float angle);
    virtual ~Drawable();
    Vector2D& getPosition();
    void setPosition(Vector2D position);
    void setX(float x);
    void setY(float y);
    float getScale();
    void setScale(float scale);
    float getRotationAngle();
    void setRotationAngle(float angle);
    Vector2D getRotationPoint();
    void setRotationPoint(Vector2D rotationPoint);
    virtual int getWidth() = 0;
    virtual int getHeight() = 0;
};

}

#endif
