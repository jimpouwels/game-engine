#ifndef gameEngine_hpp
#define gameEngine_hpp

#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <SFML/Graphics.hpp>
#include "drawable.hpp"
#include "image.hpp"
#include "sound.hpp"
#include "renderingCache.hpp"
#include "keyListener.hpp"
#include "keyboardHandler.hpp"
#include "updateThread.hpp"
#include "scroller.hpp"
#include "stageFactory.hpp"

namespace jimp {

static GameEngine* instance = nullptr;

class GameEngine {
    
private:
    uint16_t screenWidth;
    uint16_t screenHeight;
    uint32_t backgroundColor;
    int16_t frameRate;
    bool editMode = false;
    bool reloadRequested = false;
    bool reloadingStage = false;
    std::thread* reloadThread = nullptr;
    std::mutex* reloadLock = nullptr;
    std::string currentStage = "";
    std::unique_ptr<StageFactory> stageFactory = nullptr;
    GameEngine* gameEngine = nullptr;
    std::string windowTitle;
    bool isWindowClosed = false;
    std::map<std::string, Image*>* imageCache = nullptr;
    std::map<std::string, Sound*>* soundCache = nullptr;
    std::vector<std::unique_ptr<AnimatedGraphic>> allGraphics;
    std::recursive_mutex graphicsLock;
    UpdateThread* updateThread = nullptr;
    RenderingCache* renderCache = nullptr;
    float timePerFrame;
    float gravityForce;
    jimp::KeyboardHandler* keyboardHandler = nullptr;
    std::chrono::time_point<std::chrono::system_clock> previousFrameTime;
    std::chrono::time_point<std::chrono::system_clock> previousFpsMeasurementTime;
    int totalFrames;
    sf::RenderWindow* window;
    void handleReloadStageRequest();
    float measureFps(std::chrono::time_point<std::chrono::system_clock>& currentTime);
    void drawFrame(float elapsedTimeSincePreviousFrame);
    void handleEvents();
    void triggerUpdate(float elapsedTime);
    void handleSounds(float elapsedTime);
    void handleDrawableDeleted(AnimatedGraphic& graphic);
    
protected:
    GameEngine(uint16_t screenWidth, uint16_t screenHeight, float gravityForce, std::string windowTitle, uint16_t desiredFrameRate, bool editMode);
    Scroller* scroller = nullptr;
    void start();
    void setBackgroundColor(uint32_t color);
    virtual std::unique_ptr<StageFactory> getStageFactory() = 0;
    virtual void onStageLoad(std::string stageFilePath) {};
    virtual void onFrame(float elapsedTime) {};
    virtual void onUpdate(float elapsedTime) {};
    virtual void onGraphicDeleted(AnimatedGraphic* graphic) {};
    
public:
    ~GameEngine();
    static GameEngine* getInstance();
    int getScreenWidth();
    int getScreenHeight();
    void reloadCurrentStage();
    void doReloadStage(std::string filePath);
    bool isAtLeftEdgeOfScreen(AnimatedGraphic* graphic);
    bool isAtRightEdgeOfScreen(AnimatedGraphic* graphic);
    bool isAtTopEdgeOfScreen(AnimatedGraphic* graphic);
    bool isAtBottomEdgeOfScreen(AnimatedGraphic* graphic);
    bool isOutsideScreenTop(AnimatedGraphic* graphic);
    bool isOutsideScreenBottom(AnimatedGraphic* graphic);
    bool isOutsideScreenLeft(AnimatedGraphic* graphic);
    bool isOutsideScreenRight(AnimatedGraphic* graphic);
    void addKeyListener(KeyListener& keyListener);
    void draw(Drawable* drawable);
    void draw(std::string text, Vector2D position);
    void registerGraphic(std::unique_ptr<AnimatedGraphic> graphic);
    void drawRectangle(float width, float height, Vector2D position, Color color);
    float getGravityForce();
    bool isEditMode();
    bool isRunning();
    void loadStage(std::string filePath);
    struct LockedGraphics {
        std::vector<std::unique_ptr<AnimatedGraphic>>& graphics;
        std::unique_lock<std::recursive_mutex> lock;
    };
    LockedGraphics getAllGraphics();
    Image* createNewImage(std::string filePath);
    Sound* createNewSound(std::string filePath);
    
};
}

#endif
