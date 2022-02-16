#ifndef animation_hpp
#define animation_hpp

#include <vector>
#include <list>
#include <string>
#include "sprite.hpp"

namespace jimp {

class Animation {

private:
    std::string id;
    std::vector<Sprite*>* sprites;
    uint16_t activeSpriteIndex = 0;
    GameEngine* gameEngine = nullptr;
    
public:
    Animation(std::string id);
    ~Animation();
    std::string getId();
    void addSprite(Sprite* sprite);
    long getNumberOfSprites();
    Sprite* getActiveSprite();
    std::list<Sprite*> getAllSprites();
    void switchToNextSprite();
    
};

}

#endif
