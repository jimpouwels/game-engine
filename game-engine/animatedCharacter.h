#ifndef animatedCharacter_h
#define animatedCharacter_h

#include <stdio.h>
#include "sprite.h"
#include "animatedCharacter.h"
#include "drawable.h"

namespace jimp {

class AnimatedCharacter : public jimp::Drawable {

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
