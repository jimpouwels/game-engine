#ifndef animatedCharacter_hpp
#define animatedCharacter_hpp

#include <stdio.h>
#include <vector>
#include <map>
#include "sprite.hpp"

namespace jimp {

class AnimatedCharacter {

private:
    int x;
    int y;
    std::map<std::string, std::vector<jimp::Sprite*>*>* spriteMap;
    jimp::Sprite* currentSprite = nullptr;
    std::string currentSpriteCollection;
    int currentSpriteIndex;
    float elapsedTimeSinceLastSwap;
    float imageSwapIntervalInSeconds;
    
protected:
    int getX();
    void setX(int x);
    int getY();
    void setY(int y);
    void setCurrentSprite(jimp::Sprite* sprite);
    void addSprite(std::string collection, std::string filePath);
    void setNextSpriteInCollection(std::string collection, float elapsedTime);
    
public:
    AnimatedCharacter(int x, int y, float imageSwapIntervalInSeconds);
    ~AnimatedCharacter();
    jimp::Sprite& getCurrentSprite();
};
}

#endif
