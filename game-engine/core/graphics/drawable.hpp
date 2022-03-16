#ifndef drawable_hpp
#define drawable_hpp

#include <cstdint>
#include "vector2D.hpp"

namespace jimp {

class Drawable {
      
private:
    Vector2D position = Vector2D::empty();
    Vector2D rotationPoint = Vector2D::empty();
    float scale = 1.0F;
    float angle = 0.0F;
    uint16_t repeat;
    bool applyScrolling = false;
        
public:
    Drawable(float x, float y, float scale, float angle, uint16_t repeat);
    virtual ~Drawable();
    Vector2D& getPosition();
    void setPosition(Vector2D position);
    void setX(float x);
    void setY(float y);
    uint16_t getRepeat();
    float getScale();
    void setScale(float scale);
    float getRotationAngle();
    void setRotationAngle(float angle);
    Vector2D getRotationPoint();
    void setApplyScrolling(bool applyScrolling);
    bool getApplyScrolling();
    void setRotationPoint(Vector2D rotationPoint);
    bool isPositionedWithinScreen();
    virtual int getSingleWidth() = 0;
    virtual int getWidth() = 0;
    virtual int getHeight() = 0;
};

}

#endif
