#ifndef scrollingWorld_hpp
#define scrollingWorld_hpp

#include "animatedGraphic.hpp"

namespace jimp {

class GameEngine;

class ScrollingWorld {
    
private:
    AnimatedGraphic* mainCharacter = nullptr;
    bool mainCharacterLoaded = false;
    float offsetX = 0;
    float offsetY = 0;
    float maxScrollX = 0;
    float maxScrollY = 0;
    int width = 0;
    int height = 0;
    
public:
    ScrollingWorld(int width, int height);
    ~ScrollingWorld();
    int getWidth();
    int getHeight();
    void doOnUpdate();
    void doOnFrame();
    AnimatedGraphic* getMainCharacter();
    void setMainCharacterLoaded(bool loaded);
    void setMainCharacter(AnimatedGraphic* animatedGraphic);
    static ScrollingWorld* getInstance();
};

}

#endif
