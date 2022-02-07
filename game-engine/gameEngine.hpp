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
    int screenWidth;
    int screenHeight;
    int frameRate;
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
    GameEngine(int screenWidth, int screenHeight, std::string windowTitle, int desiredFrameRate);
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
