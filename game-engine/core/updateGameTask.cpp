#include "updateGameTask.hpp"
#include <iostream>
#include <chrono>

namespace jimp {

bool isUpdatingGame = false;

void doLoop(std::function<void(float)> onUpdateCallback, std::function<void(AnimatedSprite*)> onSpriteDeletedCallback, std::vector<AnimatedSprite*>* registeredSprites) {
    isUpdatingGame = true;
    std::chrono::time_point<std::chrono::system_clock> previousUpdateTime;
    while (isUpdatingGame) {
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
        std::chrono::time_point<std::chrono::system_clock> currentTime = std::chrono::system_clock::now();
        std::chrono::duration<float> elapsed = currentTime - previousUpdateTime;
        previousUpdateTime = currentTime;
        if (elapsed.count() < 1) {
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
                spriteToDelete->tryLock();
                onSpriteDeletedCallback(spriteToDelete);
                spriteToDelete->unlock();
            }
            onUpdateCallback(elapsed.count());
        }
    }
}

UpdateGameTask::UpdateGameTask(std::function<void(float)> onUpdateCallback, std::function<void(AnimatedSprite*)> onSpriteDeletedCallback) {
    this->onUpdateCallback = onUpdateCallback;
    this->onSpriteDeletedCallback = onSpriteDeletedCallback;
    this->registeredAnimatedSprites = new std::vector<AnimatedSprite*>;
}

UpdateGameTask::~UpdateGameTask() {
    isUpdatingGame = false;
    updateThread->join();
    delete updateThread;
}

void UpdateGameTask::start() {
    this->updateThread = new std::thread(doLoop, onUpdateCallback, onSpriteDeletedCallback, registeredAnimatedSprites);
}

void UpdateGameTask::stop() {
    isUpdatingGame = false;
}

std::vector<AnimatedSprite*>* UpdateGameTask::getAllSprites() {
    return registeredAnimatedSprites;
}

void UpdateGameTask::registerAnimatedSprite(AnimatedSprite* animatedSprite) {
    registeredAnimatedSprites->push_back(animatedSprite);
}

void UpdateGameTask::unregisterAnimatedSprite(AnimatedSprite* animatedSprite) {
    registeredAnimatedSprites->erase(std::remove(registeredAnimatedSprites->begin(), registeredAnimatedSprites->end(), animatedSprite), registeredAnimatedSprites->end());
    delete animatedSprite;
}

void UpdateGameTask::removeAllSprites() {
    for (const auto& sprite: *registeredAnimatedSprites) {
        delete sprite;
    }
}

}
