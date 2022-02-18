#ifndef updateThread_hpp
#define updateThread_hpp

#include <thread>
#include "animatedSprite.hpp"

namespace jimp {

class UpdateThread {
    
private:
    std::thread* updateThread = nullptr;
    std::mutex* processingLock = nullptr;
    std::function<void(float)> onUpdateCallback;
    std::function<void(AnimatedSprite*)> onSpriteDeletedCallback;
    std::vector<AnimatedSprite*>* registeredAnimatedSprites = nullptr;
    
public:
    UpdateThread(std::function<void(float)> onUpdateCallback, std::function<void(AnimatedSprite*)> onSpriteDeletedCallback);
    ~UpdateThread();
    void start();
    void stop();
    void registerAnimatedSprite(AnimatedSprite* animatedSprite);
    void unregisterAnimatedSprite(AnimatedSprite* animatedSprite);
    std::vector<AnimatedSprite*>* getAllSprites();
    void removeAllSprites();
    
};

}

#endif
