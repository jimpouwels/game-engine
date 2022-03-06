#ifndef scrollingWorld_hpp
#define scrollingWorld_hpp

#include "animatedGraphic.hpp"

namespace jimp {

class ScrollingWorld {
    
private:
    AnimatedGraphic* mainCharacter = nullptr;
    float offsetX = 0;
    float offsetY = 0;
    int width = 0;
    int height = 0;
    
public:
    ScrollingWorld(AnimatedGraphic* mainCharacter, int width, int height);
    ~ScrollingWorld();
    int getWidth();
    int getHeight();
    void doOnUpdate();
    void doOnFrame();
    static ScrollingWorld* getInstance();
};

}

#endif
