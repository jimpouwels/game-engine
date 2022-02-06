#ifndef sprite_hpp
#define sprite_hpp

#include <string>

#include "gamingInterface.hpp"
#include "vector2D.hpp"
#include "image.hpp"

namespace jimp {

class Sprite {
    
private:
    Vector2D position = { .x = 0, .y = 0};
    float scale = 1.0F;
    float angle = 0.0F;
    GamingInterface* gamingInterface;
    Image* image;
    
public:
    Sprite(GamingInterface* gamingInterface, float x, float y, float scale, std::string filePath);
    Sprite(GamingInterface* gamingInterface, float x, float y, float scale, int angle, std::string filePath);
    ~Sprite();
    Vector2D& getPosition();
    void setPosition(Vector2D position);
    void setX(float x);
    void setY(float y);
    float getScale();
    void setScale(float scale);
    float getRotationAngle();
    void setRotationAngle(float angle);
    int getWidth();
    int getHeight();
    bool isOutsideScreenBelow();
    bool isOutsideScreenAbove();
    bool isOutsideScreenLeft();
    bool isOutsideScreenRight();
    void setPosition(float x, float y);
    jimp::Image& getImage();
};
}

#endif
