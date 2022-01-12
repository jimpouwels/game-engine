#ifndef animatedCharacter_hpp
#define animatedCharacter_hpp

#include <stdio.h>
#include <vector>
#include <map>
#include "sprite.hpp"

namespace jimp {

class AnimatedCharacter {

private:
    float x;
    float y;
    std::map<std::string, std::vector<jimp::Sprite*>*>* spriteMap;
    jimp::Sprite* activeSprite = nullptr;
    std::string activeCollection;
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
    void addSprite(std::string collection, std::string filePath);
    void switchToNextSprite(std::string collection, float elapsedTime);
    
public:
    AnimatedCharacter(float x, float y, float imageSwapIntervalInSeconds);
    ~AnimatedCharacter();
    jimp::Sprite& getActiveSprite();
};
}

#endif
