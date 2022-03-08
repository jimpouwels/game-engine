#include "updateThread.hpp"
#include "scrollingWorld.hpp"
#include <iostream>
#include <chrono>

namespace jimp {

bool isWorking = false;
AnimatedGraphic* ray = nullptr;

void doLoop(std::function<void(float)> onUpdateCallback, std::function<void(AnimatedGraphic*)> onSpriteDeletedCallback, std::function<void()> onLoadNewGraphicsCallback, std::vector<AnimatedGraphic*>* registeredSprites, std::list<AnimatedGraphic*>* newGraphics, std::mutex* processingLock, std::mutex* graphicsLock) {
    isWorking = true;
    std::chrono::time_point<std::chrono::system_clock> previousUpdateTime;
    while (true) {
        onLoadNewGraphicsCallback();
        if (newGraphics->size() > 0) {
            continue;
        }
        processingLock->lock();
        if (isWorking) {
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
            std::chrono::time_point<std::chrono::system_clock> currentTime = std::chrono::system_clock::now();
            std::chrono::duration<float> elapsed = currentTime - previousUpdateTime;
            previousUpdateTime = currentTime;
            if (elapsed.count() < 1) {
                std::list<AnimatedGraphic*> spritesToDelete = std::list<AnimatedGraphic*>();
                
                for (uint16_t i = 0; i < registeredSprites->size(); i++) {
                    AnimatedGraphic* registeredSprite = registeredSprites->at(i);
                    std::vector<AnimatedGraphic*> graphicsToCheckCollision = std::vector<AnimatedGraphic*>();
                    for (uint16_t x = 0; x < registeredSprites->size(); x++) {
                        if (registeredSprites->at(x) != registeredSprite) {
                            graphicsToCheckCollision.push_back(registeredSprites->at(x));
                        }
                    }
                    ray = registeredSprite;
                    std::sort(graphicsToCheckCollision.begin(), graphicsToCheckCollision.end(), [](AnimatedGraphic* a, AnimatedGraphic* b) {
                        Vector2D distanceALeft = Vector2D {.x = 0, .y = 0};
                        Vector2D distanceARight = Vector2D {.x = 0, .y = 0};
                        Vector2D distanceBLeft = Vector2D {.x = 0, .y = 0};
                        Vector2D distanceBRight = Vector2D {.x = 0, .y = 0};
                        
                        Vector2D rayRight = Vector2D { .x = ray->getPosition().x + ray->getWidth(), .y = ray->getPosition().y };
                        distanceALeft = ray->getPosition() - a->getPosition();
                        distanceARight = rayRight- a->getPosition();
                        distanceBLeft = ray->getPosition() - b->getPosition();
                        distanceBRight = rayRight - b->getPosition();
                        
                        if (distanceALeft.x < 0) distanceALeft.x = -distanceALeft.x;
                        if (distanceALeft.y < 0) distanceALeft.y = -distanceALeft.y;
                        
                        if (distanceARight.x < 0) distanceARight.x = -distanceARight.x;
                        if (distanceARight.y < 0) distanceARight.y = -distanceARight.y;
                        
                        Vector2D nearestA = (distanceALeft.x + distanceALeft.y) < (distanceARight.x + distanceARight.y) ? distanceALeft : distanceARight;
                        
                        if (distanceBLeft.x < 0) distanceBLeft.x = -distanceBLeft.x;
                        if (distanceBLeft.y < 0) distanceBLeft.y = -distanceBLeft.y;
                        
                        if (distanceBRight.x < 0) distanceBRight.x = -distanceBRight.x;
                        if (distanceBRight.y < 0) distanceBRight.y = -distanceBRight.y;
                        
                        Vector2D nearestB = distanceBLeft.x + distanceBLeft.y < distanceBRight.x + distanceBRight.y ? distanceBLeft : distanceBRight;
                        
                        return (nearestA.x + nearestA.y) < (nearestB.x + nearestB.y);
                        
                    });
                    for (uint16_t j = 0; j < graphicsToCheckCollision.size(); j++) {
                        registeredSprite->checkCollisionRect(graphicsToCheckCollision.at(j), elapsed.count());
                    }
                    if (registeredSprite->isMarkedForDeletion()) {
                        spritesToDelete.push_back(registeredSprite);
                    }
                }
                for (uint16_t i = 0; i < registeredSprites->size(); i++) {
                    AnimatedGraphic* registeredSprite = registeredSprites->at(i);
                    if (registeredSprite->isMarkedForDeletion()) {
                        continue;
                    }
                    registeredSprite->onUpdate(elapsed.count());
                    if (registeredSprite->isMarkedForDeletion()) {
                        spritesToDelete.push_back(registeredSprite);
                    }
                }
                for (const auto& spriteToDelete: spritesToDelete) {
                    graphicsLock->lock();
                    onSpriteDeletedCallback(spriteToDelete);
                    graphicsLock->unlock();
                }
                if (ScrollingWorld::getInstance() != nullptr) {
                    ScrollingWorld::getInstance()->doOnUpdate();
                }
                onUpdateCallback(elapsed.count());
            }
        } else {
            break;
        }
        processingLock->unlock();
    }
}

UpdateThread::UpdateThread(std::function<void(float)> onUpdateCallback, std::function<void(AnimatedGraphic*)> onGraphicDeletedCallback) {
    this->processingLock = new std::mutex();
    this->graphicsLock = new std::mutex();
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
    auto onLoadNewGraphicsLambda = std::bind(&UpdateThread::loadNewGraphicsIntoUpdateLoop, this);
    this->updateThread = new std::thread(doLoop, onGraphicUpdateLambda, onGraphicDeletedLambda, onLoadNewGraphicsLambda, registeredGraphics, newGraphics, processingLock, graphicsLock);
}

void UpdateThread::stop() {
    processingLock->lock();
    isWorking = false;
    processingLock->unlock();
}

void UpdateThread::lockGraphics() {
    graphicsLock->lock();
}

void UpdateThread::unlockGraphics() {
    graphicsLock->unlock();
}

std::vector<AnimatedGraphic*>* UpdateThread::getAllGraphics() {
    return registeredGraphics;
}

void UpdateThread::registerGraphic(AnimatedGraphic* graphic) {
    newGraphics->push_back(graphic);
}

void UpdateThread::onUpdate(float elapsedTime) {
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
            newGraphic->onInit();
            registeredGraphics->push_back(newGraphic);
            loadedGraphics.push_back(newGraphic);
            if (ScrollingWorld::getInstance() != nullptr && newGraphic == ScrollingWorld::getInstance()->getMainCharacter()) {
                ScrollingWorld::getInstance()->setMainCharacterLoaded(true);
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
