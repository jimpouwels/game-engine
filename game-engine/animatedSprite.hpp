#ifndef animatedCharacter_hpp
#define animatedCharacter_hpp

#include <stdio.h>
#include <vector>
#include <map>
#include "sprite.hpp"

namespace jimp {

class AnimatedSprite {

private:
    float x;
    float y;
    int width;
    int height;
    std::map<std::string, std::vector<jimp::Sprite*>*>* spriteMap;
    jimp::Sprite* activeSprite = nullptr;
    std::string activeAnimationId;
    int activeSpriteIndex = 0;
    float elapsedTimeSinceLastSwap;
    float imageSwapIntervalInSeconds;
    void setActiveSprite(jimp::Sprite* sprite);
    void updateActiveSprite();
    
protected:
    float getX();
    void setX(float x);
    float getY();
    void setY(float y);
    void addSprite(std::string animationId, std::string filePath);
    void setCurrentAnimation(std::string animationId);
    void switchToNextSprite(float elapsedTime);
    
public:
    AnimatedSprite(float x, float y, int width, int height, float imageSwapIntervalInSeconds);
    ~AnimatedSprite();
    jimp::Sprite& getActiveSprite();
};
}

#endif
