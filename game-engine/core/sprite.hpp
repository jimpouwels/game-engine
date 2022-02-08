#ifndef sprite_hpp
#define sprite_hpp

#include <string>

#include "vector2D.hpp"
#include "image.hpp"
#include "gameEngine.hpp"

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
    Sprite(GameEngine* gameEngine, float x, float y, float scale, std::string filePath);
    Sprite(GameEngine* gameEngine, float x, float y, float scale, int angle, std::string filePath);
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
    bool isAtBottomEdgeOfScreen();
    bool isAtTopEdgeOfScreen();
    bool isAtLeftEdgeOfScreen();
    bool isAtRightEdgeOfScreen();
    bool isOutsideScreenBottom();
    bool isOutsideScreenTop();
    bool isOutsideScreenLeft();
    bool isOutsideScreenRight();
    void setPosition(float x, float y);
    void setRotationPoint(Vector2D rotationPoint);
    Vector2D getRotationPoint();
    jimp::Image& getImage();
};
}

#endif
