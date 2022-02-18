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
#include "updateThread.hpp"

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
    UpdateThread* updateThread = nullptr;
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
    void handleSounds(float elapsedTime);
    void handleSpriteDeleted(AnimatedSprite* animatedSprite);
    
protected:
    virtual void onFrame(float elapsedTime) {};
    virtual void onUpdate(float elapsedTime) {};
    virtual void onSpriteDeleted(AnimatedSprite* animatedSprite) {};
    
public:
    GameEngine(uint16_t screenWidth, uint16_t screenHeight, std::string windowTitle, uint16_t desiredFrameRate);
    ~GameEngine();
    void start();
    int getScreenWidth();
    int getScreenHeight();
    bool isAtLeftEdgeOfScreen(AnimatedSprite* animatedSprite);
    bool isAtRightEdgeOfScreen(AnimatedSprite* animatedSprite);
    bool isAtTopEdgeOfScreen(AnimatedSprite* animatedSprite);
    bool isAtBottomEdgeOfScreen(AnimatedSprite* animatedSprite);
    bool isOutsideScreenTop(AnimatedSprite* animatedSprite);
    bool isOutsideScreenBottom(AnimatedSprite* animatedSprite);
    bool isOutsideScreenLeft(AnimatedSprite* animatedSprite);
    bool isOutsideScreenRight(AnimatedSprite* animatedSprite);
    bool isPositionWithinScreen(Vector2D position);
    void addKeyListener(KeyListener* keyListener);
    void draw(Sprite* sprite);
    void registerAnimatedSprite(AnimatedSprite* animatedSprite);
    Image* registerImage(Image* image);
    Sound* registerSound(Sound* sound);
    
};
}

#endif
