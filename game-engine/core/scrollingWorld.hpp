#ifndef scrollingWorld_hpp
#define scrollingWorld_hpp

#include "animatedGraphic.hpp"

namespace jimp {

class ScrollingWorld {
    
private:
    AnimatedGraphic* mainCharacter = nullptr;
    int offsetX = 0;
    int width = 0;
    int height = 0;
    
public:
    ScrollingWorld(AnimatedGraphic* mainCharacter, int width, int height);
    ~ScrollingWorld();
    void doOnUpdate();
    void doOnFrame();
};

}

#endif
