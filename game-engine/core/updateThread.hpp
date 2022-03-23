#ifndef updateThread_hpp
#define updateThread_hpp

#include <thread>
#include "animatedGraphic.hpp"
#include "keyboardHandler.hpp"

namespace jimp {

class UpdateThread {
    
private:
    std::thread* updateThread = nullptr;
    std::recursive_mutex* processingLock = nullptr;
    std::mutex* graphicsDeletionLock = nullptr;
    std::function<void(float)> onUpdateCallback;
    std::function<void(AnimatedGraphic*)> onGraphicDeletedCallback;
    std::vector<AnimatedGraphic*>* registeredGraphics = nullptr;
    std::list<AnimatedGraphic*>* newGraphics = nullptr;
    KeyboardHandler* keyboardHandler = nullptr;
    void onUpdate(float elapsedTime);
    void onGraphicDeleted(AnimatedGraphic* graphic);
    
public:
    UpdateThread(std::function<void(float)> onUpdateCallback, std::function<void(AnimatedGraphic*)> onGraphicDeletedCallback, KeyboardHandler* keyboardHandler);
    ~UpdateThread();
    void stop();
    void start();
    void pause();
    void unpause();
    void registerGraphic(AnimatedGraphic* graphic);
    void lockDeletionOfGraphics();
    void unlockDeletionOfGraphics();
    std::vector<AnimatedGraphic*>* getAllGraphics();
    void removeAllGraphics();
    
};

}

#endif
