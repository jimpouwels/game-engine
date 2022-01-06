#ifndef animatedCharacter_h
#define animatedCharacter_h

#include <stdio.h>
#include "sprite.h"

namespace jimp {

class AnimatedCharacter : public jimp::Sprite {

private:
    Sprite *sprite;
    
public:
    AnimatedCharacter(Sprite *sprite);
    ~AnimatedCharacter();
    std::string getFilePath();
    int getX();
    int getY();
};
}

#endif
