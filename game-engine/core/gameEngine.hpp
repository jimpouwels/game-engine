#ifndef gameEngine_hpp
#define gameEngine_hpp

#include <iostream>
#include <map>
#include <SFML/Graphics.hpp>
#include "sprite.hpp"
#include "spriteCache.hpp"
#include "keyListener.hpp"
#include "keyboardHandler.hpp"

namespace jimp {

class GameEngine {
    
private:
    uint16_t screenWidth;
    uint16_t screenHeight;
    int16_t frameRate;
    GameEngine* gameEngine = nullptr;
    std::string windowTitle;
    std::map<std::string, Image*>* imageCache = nullptr;
    SpriteCache* spriteCache = nullptr;
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
    GameEngine(uint16_t screenWidth, uint16_t screenHeight, std::string windowTitle, uint16_t desiredFrameRate);
    ~GameEngine();
    void start();
    void draw(Sprite* sprite);
    void eraseFromCache(Sprite* sprite);
    int getScreenWidth();
    int getScreenHeight();
    bool isPositionWithinScreen(float x, float y);
    void addKeyListener(jimp::KeyListener* keyListener);
    Image* loadImage(std::string filePath);
    virtual void onFrame(float elapsedTime) = 0;
    
};
}

#endif
