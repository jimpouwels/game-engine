#include "updateThread.hpp"
#include <iostream>
#include <chrono>

namespace jimp {

bool isWorking = false;

void doLoop(std::function<void(float)> onUpdateCallback, std::function<void(AnimatedSprite*)> onSpriteDeletedCallback, std::vector<AnimatedSprite*>* registeredSprites, std::mutex* processingLock, std::mutex* deleteSpriteLock) {
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
                    deleteSpriteLock->lock();
                    onSpriteDeletedCallback(spriteToDelete);
                    deleteSpriteLock->unlock();
                }
            }
        } else {
            break;
        }
        processingLock->unlock();
    }
}

UpdateThread::UpdateThread(std::function<void(float)> onUpdateCallback, std::function<void(AnimatedSprite*)> onSpriteDeletedCallback) {
    this->processingLock = new std::mutex();
    this->deleteSpriteLock = new std::mutex();
    this->onUpdateCallback = onUpdateCallback;
    this->onSpriteDeletedCallback = onSpriteDeletedCallback;
    this->registeredAnimatedSprites = new std::vector<AnimatedSprite*>;
    this->addedAnimatedSprites = new std::list<AnimatedSprite*>;
}

UpdateThread::~UpdateThread() {
    stop();
    for (const auto& sprite: *registeredAnimatedSprites) {
        delete sprite;
    }
    delete registeredAnimatedSprites;
    updateThread->join();
    delete updateThread;
    delete processingLock;
}

void UpdateThread::start() {
    auto onSpriteDeletedLambda = std::bind(&UpdateThread::onSpriteDeleted, this, std::placeholders::_1);
    auto onSpriteUpdateLambda = std::bind(&UpdateThread::onUpdate, this, std::placeholders::_1);
    this->updateThread = new std::thread(doLoop, onSpriteUpdateLambda, onSpriteDeletedLambda, registeredAnimatedSprites, processingLock, deleteSpriteLock);
}

void UpdateThread::stop() {
    processingLock->lock();
    isWorking = false;
    processingLock->unlock();
}

void UpdateThread::lockForDeletion() {
    deleteSpriteLock->lock();
}

void UpdateThread::unlockForDeletion() {
    deleteSpriteLock->unlock();
}

std::vector<AnimatedSprite*>* UpdateThread::getAllSprites() {
    return registeredAnimatedSprites;
}

void UpdateThread::registerAnimatedSprite(AnimatedSprite* animatedSprite) {
    addedAnimatedSprites->push_back(animatedSprite);
}

void UpdateThread::onUpdate(float elapsedTime) {
    if (addedAnimatedSprites->size() > 0) {
        std::list<AnimatedSprite*> completedSprites = std::list<AnimatedSprite*>();
        for (const auto& addedSprite: *addedAnimatedSprites) {
            if (addedSprite->isInitialized()) {
                registeredAnimatedSprites->push_back(addedSprite);
                completedSprites.push_back(addedSprite);
            }
        }
        if (completedSprites.size() > 0) {
            for (const auto& completedSprite: completedSprites) {
                addedAnimatedSprites->remove(completedSprite);
            }
            std::sort(registeredAnimatedSprites->begin(), registeredAnimatedSprites->end(), [](AnimatedSprite* a, AnimatedSprite* b) {
                 return a->getZIndex() > b->getZIndex();
            });
        }
    }
    this->onUpdateCallback(elapsedTime);
}

void UpdateThread::onSpriteDeleted(AnimatedSprite* animatedSprite) {
    onSpriteDeletedCallback(animatedSprite);
    registeredAnimatedSprites->erase(std::remove(registeredAnimatedSprites->begin(), registeredAnimatedSprites->end(), animatedSprite), registeredAnimatedSprites->end());
    delete animatedSprite;
}

}
