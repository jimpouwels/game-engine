#ifndef updateThread_hpp
#define updateThread_hpp

#include <thread>
#include "graphic.hpp"

namespace jimp {

class UpdateThread {
    
private:
    std::thread* updateThread = nullptr;
    std::mutex* processingLock = nullptr;
    std::mutex* deleteGraphicLock = nullptr;
    std::function<void(float)> onUpdateCallback;
    std::function<void(Graphic*)> onGraphicDeletedCallback;
    std::vector<Graphic*>* registeredGraphics = nullptr;
    std::list<Graphic*>* newGraphics = nullptr;
    void stop();
    void loadNewGraphicsIntoUpdateLoop();
    void onUpdate(float elapsedTime);
    void onGraphicDeleted(Graphic* graphic);
    
public:
    UpdateThread(std::function<void(float)> onUpdateCallback, std::function<void(Graphic*)> onGraphicDeletedCallback);
    ~UpdateThread();
    void start();
    void registerGraphic(Graphic* graphic);
    void lockForDeletion();
    void unlockForDeletion();
    std::vector<Graphic*>* getAllGraphics();
    void removeAllGraphics();
    
};

}

#endif
