#ifndef scrollingWorld_hpp
#define scrollingWorld_hpp

#include "animatedGraphic.hpp"

namespace mario {

class ScrollingWorld {
    
private:
    jimp::AnimatedGraphic* mainCharacter = nullptr;
    int offsetX = 0;
    int width = 0;
    int height = 0;
    
public:
    ScrollingWorld(jimp::AnimatedGraphic* mainCharacter, int width, int height);
    ~ScrollingWorld();
    void doOnUpdate();
    void doOnFrame();
};

}

#endif
