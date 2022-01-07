#ifndef animatedCharacter_hpp
#define animatedCharacter_hpp

#include <stdio.h>
#include "sprite.hpp"

namespace jimp {

class AnimatedCharacter : public jimp::Sprite {

private:
    Sprite *sprite;
    
public:
    AnimatedCharacter(Sprite* sprite);
    ~AnimatedCharacter();
    std::string getFilePath();
    int getX();
    int getY();
};
}

#endif
