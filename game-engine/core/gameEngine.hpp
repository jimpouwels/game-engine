#ifndef gameEngine_hpp
#define gameEngine_hpp

#include <iostream>
#include <map>
#include <SFML/Graphics.hpp>
#include "drawable.hpp"
#include "image.hpp"
#include "sound.hpp"
#include "spriteCache.hpp"
#include "keyListener.hpp"
#include "keyboardHandler.hpp"
#include "updateThread.hpp"

namespace jimp {

static GameEngine* instance = nullptr;

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
    void handleDrawableDeleted(AnimatedGraphic* graphic);
    
protected:
    GameEngine(uint16_t screenWidth, uint16_t screenHeight, std::string windowTitle, uint16_t desiredFrameRate);
    void start();
    virtual void onFrame(float elapsedTime) {};
    virtual void onUpdate(float elapsedTime) {};
    virtual void onGraphicDeleted(AnimatedGraphic* graphic) {};
    
public:
    ~GameEngine();
    static GameEngine* getInstance();
    int getScreenWidth();
    int getScreenHeight();
    bool isAtLeftEdgeOfScreen(AnimatedGraphic* graphic);
    bool isAtRightEdgeOfScreen(AnimatedGraphic* graphic);
    bool isAtTopEdgeOfScreen(AnimatedGraphic* graphic);
    bool isAtBottomEdgeOfScreen(AnimatedGraphic* graphic);
    bool isOutsideScreenTop(AnimatedGraphic* graphic);
    bool isOutsideScreenBottom(AnimatedGraphic* graphic);
    bool isOutsideScreenLeft(AnimatedGraphic* graphic);
    bool isOutsideScreenRight(AnimatedGraphic* graphic);
    bool isPositionWithinScreen(Vector2D position);
    void addKeyListener(KeyListener* keyListener);
    void draw(Drawable* drawable);
    void registerGraphic(AnimatedGraphic* graphic);
    void drawRectangle(float width, float height, Vector2D position);
    Image* registerImage(Image* image);
    Sound* registerSound(Sound* sound);
    
};
}

#endif
