#ifndef animation_hpp
#define animation_hpp

#include <vector>
#include <string>
#include "sprite.hpp"
#include "gameEngine.hpp"

namespace jimp {

class Animation {

private:
    std::string id;
    std::vector<Sprite*>* sprites;
    uint16_t activeSpriteIndex = 0;
    GameEngine* gameEngine = nullptr;
    
public:
    Animation(GameEngine* gameEngine, std::string id);
    ~Animation();
    std::string getId();
    void addSprite(Sprite* sprite);
    long getNumberOfSprites();
    Sprite* getActiveSprite();
    void switchToNextSprite();
    
};

}

#endif
