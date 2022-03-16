#ifndef updateThread_hpp
#define updateThread_hpp

#include <thread>
#include "animatedGraphic.hpp"

namespace jimp {

class UpdateThread {
    
private:
    std::thread* updateThread = nullptr;
    std::recursive_mutex* processingLock = nullptr;
    std::mutex* graphicsLock = nullptr;
    std::function<void(float)> onUpdateCallback;
    std::function<void(AnimatedGraphic*)> onGraphicDeletedCallback;
    std::vector<AnimatedGraphic*>* registeredGraphics = nullptr;
    std::list<AnimatedGraphic*>* newGraphics = nullptr;
    void stop();
    void onUpdate(float elapsedTime);
    void onGraphicDeleted(AnimatedGraphic* graphic);
    
public:
    UpdateThread(std::function<void(float)> onUpdateCallback, std::function<void(AnimatedGraphic*)> onGraphicDeletedCallback);
    ~UpdateThread();
    void start();
    void pause();
    void unpause();
    void registerGraphic(AnimatedGraphic* graphic);
    void lockDeletionOfGraphics();
    void unlockGraphics();
    std::vector<AnimatedGraphic*>* getAllGraphics();
    void removeAllGraphics();
    
};

}

#endif
