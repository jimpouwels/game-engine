#ifndef animation_hpp
#define animation_hpp

#include <vector>
#include "sprite.hpp"

namespace jimp {

class Animation {
  
private:
    std::string id;
    std::vector<jimp::Sprite*>* sprites;
    
public:
    Animation(std::string id);
    ~Animation();
    void addSprite(Sprite* sprite);
    long getNumberOfSprites();
    jimp::Sprite* getSpriteAt(int index);
    
};

}

#endif
