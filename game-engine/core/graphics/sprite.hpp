#ifndef sprite_hpp
#define sprite_hpp

#include <string>

#include "vector2D.hpp"
#include "image.hpp"

namespace jimp {

class GameEngine;

class Sprite {
    
private:
    Vector2D position = { .x = 0, .y = 0};
    Vector2D rotationPoint = { .x = 0, .y = 0 };
    float scale = 1.0F;
    float angle = 0.0F;
    GameEngine* gameEngine;
    Image* image;
    
public:
    Sprite(float x, float y, float scale, std::string filePath);
    Sprite(float x, float y, float scale, uint16_t angle, std::string filePath);
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
    void setPosition(float x, float y);
    void setRotationPoint(Vector2D rotationPoint);
    Vector2D getRotationPoint();
    jimp::Image& getImage();
};
}

#endif
