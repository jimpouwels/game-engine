#ifndef animatedCharacter_hpp
#define animatedCharacter_hpp

#include <stdio.h>
#include <vector>
#include <map>
#include "sprite.hpp"
#include "animation.hpp"
#include "gameEngine.hpp"

namespace jimp {

class AnimatedSprite {

private:
    float x;
    float y;
    float scale;
    jimp::GameEngine* gameEngine;
    std::map<std::string, jimp::Animation*>* animationMap;
    jimp::Animation* activeAnimation = nullptr;
    float elapsedTimeSinceLastSwap;
    float imageSwapIntervalInSeconds;
    
protected:
    void setX(float x);
    void setY(float y);
    void updateAnimation(float elapsedTime);
    void setCurrentAnimation(std::string animationId);
    
public:
    float getX();
    float getY();
    int getWidth();
    int getHeight();
    void rotate(float angle);
    float getRotationAngle();
    void setRotationAngle(float angle);
    bool isPositionedWithinScreen();
    GameEngine& getGameEngine();
    AnimatedSprite(jimp::GameEngine* gameEngine, float x, float y, float scale, float imageSwapIntervalInSeconds);
    AnimatedSprite(jimp::GameEngine* gameEngine, float x, float y, float scale, int rotationAngle, float imageSwapIntervalInSeconds);
    ~AnimatedSprite();
    jimp::Sprite& getActiveSprite();
    virtual void update(float elapsedTime) {
        this->updateAnimation(elapsedTime);
    };
    void addSprite(std::string animationId, std::string filePath);
};
}

#endif
