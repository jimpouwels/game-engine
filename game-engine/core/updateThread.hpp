#ifndef updateThread_hpp
#define updateThread_hpp

#include <thread>
#include "animatedSprite.hpp"

namespace jimp {

class UpdateThread {
    
private:
    std::thread* updateThread = nullptr;
    std::mutex* processingLock = nullptr;
    std::mutex* deleteSpriteLock = nullptr;
    std::function<void(float)> onUpdateCallback;
    std::function<void(AnimatedSprite*)> onSpriteDeletedCallback;
    std::vector<AnimatedSprite*>* registeredAnimatedSprites = nullptr;
    std::list<AnimatedSprite*>* addedAnimatedSprites = nullptr;
    void stop();
    void onUpdate(float elapsedTime);
    void onSpriteDeleted(AnimatedSprite* animatedSprite);
    
public:
    UpdateThread(std::function<void(float)> onUpdateCallback, std::function<void(AnimatedSprite*)> onSpriteDeletedCallback);
    ~UpdateThread();
    void start();
    void registerAnimatedSprite(AnimatedSprite* animatedSprite);
    void lockForDeletion();
    void unlockForDeletion();
    std::vector<AnimatedSprite*>* getAllSprites();
    void removeAllSprites();
    
};

}

#endif
