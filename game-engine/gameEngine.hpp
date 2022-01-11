#ifndef gameEngine_hpp
#define gameEngine_hpp

#include <iostream>
#include <SFML/Graphics.hpp>
#include "sprite.hpp"
#include "keyListener.hpp"
#include "keyboardHandler.hpp"

namespace jimp {

class GameEngine {
    
private:
    int screenWidth;
    int screenHeight;
    int frameRate;
    std::string windowTitle;
    float pixelSize;
    float timePerFrame;
    jimp::KeyboardHandler* keyboardHandler = nullptr;
    std::chrono::time_point<std::chrono::system_clock> previousFrameTime;
    std::chrono::time_point<std::chrono::system_clock> previousFpsMeasurementTime;
    int totalFrames;
    sf::RenderWindow* window;
    float measureFps(std::chrono::time_point<std::chrono::system_clock>& currentTime);
    void drawFrame(std::chrono::duration<float>& elapsedTimeSincePreviousFrame, std::chrono::time_point<std::chrono::system_clock>& currentTime);
    void handleEvents();
    
public:
    GameEngine(int screenWidth, int screenHeight, std::string windowTitle, int desiredFrameRate, float pixelSize);
    ~GameEngine();
    void start();
    void draw(jimp::Sprite& sprite);
    int getScreenWidth();
    int getScreenHeight();
    void addKeyListener(jimp::KeyListener* keyListener);
    virtual void onFrame(float elapsedTime) = 0;
    
};
}

#endif
