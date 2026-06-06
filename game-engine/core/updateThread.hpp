#ifndef updateThread_hpp
#define updateThread_hpp

#include <thread>
#include <memory>
#include "animatedGraphic.hpp"
#include "keyboardHandler.hpp"

namespace jimp {

class GameEngine;

class UpdateThread {
    
private:
    std::thread* updateThread = nullptr;
    std::recursive_mutex* processingLock = nullptr;
    std::function<void(float)> onUpdateCallback;
    std::function<void(AnimatedGraphic&)> onGraphicDeletedCallback;
    GameEngine* gameEngine = nullptr;
    KeyboardHandler* keyboardHandler = nullptr;
    void onUpdate(float elapsedTime);
    void onGraphicDeleted(AnimatedGraphic& graphic);
    
public:
    UpdateThread(std::function<void(float)> onUpdateCallback, std::function<void(AnimatedGraphic&)> onGraphicDeletedCallback, GameEngine* gameEngine, KeyboardHandler* keyboardHandler);
    ~UpdateThread();
    void stop();
    void start();
    void pause();
    void unpause();
    void removeAllGraphics();
    
};

}

#endif
