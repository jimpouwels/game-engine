#ifndef updateGameTask_hpp
#define updateGameTask_hpp

#include <thread>
#include "animatedSprite.hpp"

namespace jimp {

class UpdateGameTask {
    
private:
    std::thread* updateThread = nullptr;
    std::mutex* processingLock = nullptr;
    std::function<void(float)> onUpdateCallback;
    std::function<void(AnimatedSprite*)> onSpriteDeletedCallback;
    std::vector<AnimatedSprite*>* registeredAnimatedSprites = nullptr;
    
public:
    UpdateGameTask(std::function<void(float)> onUpdateCallback, std::function<void(AnimatedSprite*)> onSpriteDeletedCallback);
    ~UpdateGameTask();
    void start();
    void stop();
    void registerAnimatedSprite(AnimatedSprite* animatedSprite);
    void unregisterAnimatedSprite(AnimatedSprite* animatedSprite);
    std::vector<AnimatedSprite*>* getAllSprites();
    void removeAllSprites();
    
};

}

#endif
