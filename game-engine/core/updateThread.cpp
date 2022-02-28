#include "updateThread.hpp"
#include <iostream>
#include <chrono>

namespace jimp {

bool isWorking = false;

void doLoop(std::function<void(float)> onUpdateCallback, std::function<void(AnimatedGraphic*)> onSpriteDeletedCallback, std::vector<AnimatedGraphic*>* registeredSprites, std::mutex* processingLock, std::mutex* deleteSpriteLock) {
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
                std::list<AnimatedGraphic*> spritesToDelete = std::list<AnimatedGraphic*>();
                for (uint16_t i = 0; i < registeredSprites->size(); i++) {
                    AnimatedGraphic* registeredSprite = registeredSprites->at(i);
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

UpdateThread::UpdateThread(std::function<void(float)> onUpdateCallback, std::function<void(AnimatedGraphic*)> onGraphicDeletedCallback) {
    this->processingLock = new std::mutex();
    this->deleteGraphicLock = new std::mutex();
    this->onUpdateCallback = onUpdateCallback;
    this->onGraphicDeletedCallback = onGraphicDeletedCallback;
    this->registeredGraphics = new std::vector<AnimatedGraphic*>;
    this->newGraphics = new std::list<AnimatedGraphic*>;
}

UpdateThread::~UpdateThread() {
    stop();
    for (const auto& sprite: *registeredGraphics) {
        delete sprite;
    }
    delete registeredGraphics;
    updateThread->join();
    delete updateThread;
    delete processingLock;
}

void UpdateThread::start() {
    auto onGraphicDeletedLambda = std::bind(&UpdateThread::onGraphicDeleted, this, std::placeholders::_1);
    auto onGraphicUpdateLambda = std::bind(&UpdateThread::onUpdate, this, std::placeholders::_1);
    this->updateThread = new std::thread(doLoop, onGraphicUpdateLambda, onGraphicDeletedLambda, registeredGraphics, processingLock, deleteGraphicLock);
}

void UpdateThread::stop() {
    processingLock->lock();
    isWorking = false;
    processingLock->unlock();
}

void UpdateThread::lockForDeletion() {
    deleteGraphicLock->lock();
}

void UpdateThread::unlockForDeletion() {
    deleteGraphicLock->unlock();
}

std::vector<AnimatedGraphic*>* UpdateThread::getAllGraphics() {
    return registeredGraphics;
}

void UpdateThread::registerGraphic(AnimatedGraphic* graphic) {
    newGraphics->push_back(graphic);
}

void UpdateThread::onUpdate(float elapsedTime) {
    this->loadNewGraphicsIntoUpdateLoop();
    this->onUpdateCallback(elapsedTime);
}

void UpdateThread::onGraphicDeleted(AnimatedGraphic* graphic) {
    onGraphicDeletedCallback(graphic);
    registeredGraphics->erase(std::remove(registeredGraphics->begin(), registeredGraphics->end(), graphic), registeredGraphics->end());
    delete graphic;
}

void UpdateThread::loadNewGraphicsIntoUpdateLoop() {
    if (newGraphics->size() > 0) {
        std::list<AnimatedGraphic*> loadedGraphics = std::list<AnimatedGraphic*>();
        for (const auto& newGraphic: *newGraphics) {
            if (newGraphic->isInitialized()) {
                newGraphic->onInit();
                registeredGraphics->push_back(newGraphic);
                loadedGraphics.push_back(newGraphic);
            }
        }
        if (loadedGraphics.size() > 0) {
            for (const auto& loadedGraphic: loadedGraphics) {
                newGraphics->remove(loadedGraphic);
            }
            std::sort(registeredGraphics->begin(), registeredGraphics->end(), [](AnimatedGraphic* a, AnimatedGraphic* b) {
                return a->getZIndex() > b->getZIndex();
            });
        }
    }
}

}
