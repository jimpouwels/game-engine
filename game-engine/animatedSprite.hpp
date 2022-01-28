#ifndef animatedCharacter_hpp
#define animatedCharacter_hpp

#include <stdio.h>
#include <vector>
#include <map>
#include "sprite.hpp"
#include "animation.hpp"
#include "screen.hpp"

namespace jimp {

class AnimatedSprite {

private:
    float x;
    float y;
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
    void updateAnimation(float elapsedTime);
    void setCurrentAnimation(std::string animationId);
    
public:
    Screen& getScreen();
    float getX();
    float getY();
    float getScale();
    int getWidth();
    int getHeight();
    float getRotationAngle();
    void setRotationAngle(float angle);
    bool isPositionedWithinScreen();
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
