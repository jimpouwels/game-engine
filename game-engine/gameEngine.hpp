#ifndef gameEngine_hpp
#define gameEngine_hpp

#include <iostream>
#include <SFML/Graphics.hpp>
#include "sprite.hpp"
#include "animatedSprite.hpp"
#include "keyListener.hpp"
#include "keyboardHandler.hpp"

namespace jimp {

class GameEngine {
    
private:
    int screenWidth;
    int screenHeight;
    int frameRate;
    std::string windowTitle;
    float timePerFrame;
    jimp::KeyboardHandler* keyboardHandler = nullptr;
    std::chrono::time_point<std::chrono::system_clock> previousFrameTime;
    std::chrono::time_point<std::chrono::system_clock> previousFpsMeasurementTime;
    int totalFrames;
    sf::RenderWindow* window;
    float measureFps(std::chrono::time_point<std::chrono::system_clock>& currentTime);
    void drawFrame(float elapsedTimeSincePreviousFrame);
    void handleEvents();
    
public:
    GameEngine(int screenWidth, int screenHeight, std::string windowTitle, int desiredFrameRate);
    ~GameEngine();
    void start();
    void draw(jimp::Sprite& sprite);
    int getScreenWidth();
    int getScreenHeight();
    bool isPositionWithinScreen(float x, float y);
    void addKeyListener(jimp::KeyListener* keyListener);
    virtual void onFrame(float elapsedTime) = 0;
    
};
}

#endif
