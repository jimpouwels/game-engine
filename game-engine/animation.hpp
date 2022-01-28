#ifndef animation_hpp
#define animation_hpp

#include <vector>
#include <string>
#include "sprite.hpp"

namespace jimp {

class Animation {
  
private:
    std::string id;
    std::vector<Sprite*>* sprites;
    int activeSpriteIndex = 0;
    
public:
    Animation(std::string id);
    ~Animation();
    std::string getId();
    void addSprite(Sprite* sprite);
    long getNumberOfSprites();
    Sprite* getActiveSprite();
    void switchToNextSprite();
    
};

}

#endif
