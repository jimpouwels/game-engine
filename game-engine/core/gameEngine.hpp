#ifndef gameEngine_hpp
#define gameEngine_hpp

#include <iostream>
#include <map>
#include <SFML/Graphics.hpp>
#include "sprite.hpp"
#include "sound.hpp"
#include "spriteCache.hpp"
#include "keyListener.hpp"
#include "keyboardHandler.hpp"
#include "updateGameTask.hpp"

namespace jimp {

class GameEngine {
    
private:
    uint16_t screenWidth;
    uint16_t screenHeight;
    int16_t frameRate;
    GameEngine* gameEngine = nullptr;
    std::string windowTitle;
    bool isWindowClosed = false;
    std::map<std::string, Image*>* imageCache = nullptr;
    std::map<std::string, Sound*>* soundCache = nullptr;
    UpdateGameTask* updateGameTask = nullptr;
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
    void triggerUpdate(float elapsedTime);
    void updateLoop(GameEngine* gameEngine);
    void cleanupOldSoundRuns();
    
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
    Sound* loadSound(std::string filePath);
    virtual void onFrame(float elapsedTime) {};
    virtual void onUpdate(float elapsedTime) {};
    
};
}

#endif
