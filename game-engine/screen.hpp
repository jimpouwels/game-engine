#ifndef screen_hpp
#define screen_hpp

#include <stdio.h>

namespace jimp {

// forward declare GameEngine to resolve circular dependency
class GameEngine;
class Sprite;

class Screen {
    
private:
    jimp::GameEngine* gameEngine;
    
public:
    Screen(GameEngine* gameEngine);
    ~Screen();
    void draw(Sprite* sprite);
    int getWidth();
    int getHeight();
    bool isWithin(float x, float y);
};
}

#endif
