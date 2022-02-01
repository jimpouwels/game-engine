#ifndef animatedCharacter_hpp
#define animatedCharacter_hpp

#include <stdio.h>
#include <vector>
#include <map>
#include "sprite.hpp"
#include "animation.hpp"
#include "screen.hpp"
#include "point2D.hpp"

namespace jimp {

class AnimatedSprite {

private:
    Point2D position = { .x = 0, .y = 0 };
    float scale;
    float angle;
    Screen* screen;
    std::map<std::string, Animation*>* animationMap;
    Animation* activeAnimation = nullptr;
    float elapsedTimeSinceLastSwap;
    float imageSwapIntervalInSeconds;
    
protected:
    void setX(float x);
    void setY(float y);
    void setPosition(Point2D position);
    void addToPosition(Point2D delta);
    void updateAnimation(float elapsedTime);
    void setCurrentAnimation(std::string animationId);
    
public:
    Screen& getScreen();
    Point2D& getPosition();
    float getScale();
    int getWidth();
    int getHeight();
    float getRotationAngle();
    void setRotationAngle(float angle);
    bool isPositionedWithinScreen();
    bool isOutsideScreenBelow();
    bool isOutsideScreenAbove();
    bool isOutsideScreenLeft();
    bool isOutsideScreenRight();
    bool isFacingUp();
    bool isFacingDown();
    bool isFacingLeft();
    bool isFacingRight();
    AnimatedSprite(Screen* screen, float x, float y, float scale, int rotationAngle, float imageSwapIntervalInSeconds);
    AnimatedSprite(Screen* screen, float x, float y, float scale, float imageSwapIntervalInSeconds);
    ~AnimatedSprite();
    Sprite& getActiveSprite();
    virtual void update(float elapsedTime) {
        this->updateAnimation(elapsedTime);
    };
    void addSprite(std::string animationId, std::string filePath);
};
}

#endif
