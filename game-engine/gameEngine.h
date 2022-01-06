#ifndef gameEngine_h
#define gameEngine_h

#include <iostream>
#include <SFML/Graphics.hpp>
#include "sprite.h"

namespace jimp {

class GameEngine {
    
private:
    int screenWidth;
    int screenHeight;
    int frameRate;
    float pixelSize;
    float timePerFrame;
    std::chrono::time_point<std::chrono::system_clock> previousFrameTime;
    sf::RenderWindow* window;
    
public:
    GameEngine(int screenWidth, int screenHeight, std::string windowTitle, int desiredFrameRate, float pixelSize);
    ~GameEngine();
    void start();
    void draw(jimp::Sprite sprite);
    int getScreenWidth();
    int getScreenHeight();
    virtual void onFrame(int elapsedTime) = 0;
};
}

#endif
