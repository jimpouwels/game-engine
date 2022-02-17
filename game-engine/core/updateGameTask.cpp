#include "updateGameTask.hpp"
#include <iostream>
#include <chrono>

namespace jimp {

bool isWorking = false;

void doLoop(std::function<void(float)> onUpdateCallback, std::function<void(AnimatedSprite*)> onSpriteDeletedCallback, std::vector<AnimatedSprite*>* registeredSprites, std::mutex* processingLock) {
    isWorking = true;
    std::chrono::time_point<std::chrono::system_clock> previousUpdateTime;
    while (true) {
        processingLock->lock();
        if (isWorking) {
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
            std::chrono::time_point<std::chrono::system_clock> currentTime = std::chrono::system_clock::now();
            std::chrono::duration<float> elapsed = currentTime - previousUpdateTime;
            previousUpdateTime = currentTime;
            if (elapsed.count() < 1) {
                onUpdateCallback(elapsed.count());
                std::list<AnimatedSprite*> spritesToDelete = std::list<AnimatedSprite*>();
                for (uint16_t i = 0; i < registeredSprites->size(); i++) {
                    AnimatedSprite* registeredSprite = registeredSprites->at(i);
                    for (uint16_t j = i + 1; j < registeredSprites->size(); j++) {
                        registeredSprite->checkCollisionRect(registeredSprites->at(j));
                    }
                    if (registeredSprite->isMarkedForDeletion()) {
                        spritesToDelete.push_back(registeredSprite);
                    } else {
                        registeredSprite->onUpdate(elapsed.count());
                        if (registeredSprite->isMarkedForDeletion()) {
                            spritesToDelete.push_back(registeredSprite);
                        }
                    }
                }
                for (const auto& spriteToDelete: spritesToDelete) {
                    spriteToDelete->lock();
                    onSpriteDeletedCallback(spriteToDelete);
                    spriteToDelete->unlock();
                }
            }
        } else {
            break;
        }
        processingLock->unlock();
    }
}

UpdateGameTask::UpdateGameTask(std::function<void(float)> onUpdateCallback, std::function<void(AnimatedSprite*)> onSpriteDeletedCallback) {
    this->processingLock = new std::mutex();
    this->onUpdateCallback = onUpdateCallback;
    this->onSpriteDeletedCallback = onSpriteDeletedCallback;
    this->registeredAnimatedSprites = new std::vector<AnimatedSprite*>;
}

UpdateGameTask::~UpdateGameTask() {
    for (const auto& sprite: *registeredAnimatedSprites) {
        delete sprite;
    }
    updateThread->join();
    delete updateThread;
    delete processingLock;
}

void UpdateGameTask::start() {
    this->updateThread = new std::thread(doLoop, onUpdateCallback, onSpriteDeletedCallback, registeredAnimatedSprites, processingLock);
}

void UpdateGameTask::stop() {
    processingLock->lock();
    isWorking = false;
    processingLock->unlock();
}

std::vector<AnimatedSprite*>* UpdateGameTask::getAllSprites() {
    return registeredAnimatedSprites;
}

void UpdateGameTask::registerAnimatedSprite(AnimatedSprite* animatedSprite) {
    registeredAnimatedSprites->push_back(animatedSprite);
    std::sort(registeredAnimatedSprites->begin(), registeredAnimatedSprites->end(), [](AnimatedSprite* a, AnimatedSprite* b) {
         return a->getZIndex() > b->getZIndex();
    });
}

void UpdateGameTask::unregisterAnimatedSprite(AnimatedSprite* animatedSprite) {
    registeredAnimatedSprites->erase(std::remove(registeredAnimatedSprites->begin(), registeredAnimatedSprites->end(), animatedSprite), registeredAnimatedSprites->end());
    delete animatedSprite;
}

}
