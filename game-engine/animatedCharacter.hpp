#ifndef animatedCharacter_hpp
#define animatedCharacter_hpp

#include <stdio.h>
#include "sprite.hpp"

namespace jimp {

class AnimatedCharacter : public jimp::Sprite {

protected:
    
public:
    AnimatedCharacter(int x, int y, std::string filePath);
    ~AnimatedCharacter();
};
}

#endif
