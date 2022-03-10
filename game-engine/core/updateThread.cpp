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
                        
                        Vector2D rayLeft = Vector2D { .x = ray->getPosition().x + ray->getMarginLeft(), .y = ray->getPosition().y + ray->getMarginTop() };
                        Vector2D rayRight = Vector2D { .x = ray->getPosition().x + ray->getWidth() - ray->getMarginRight(), .y = ray->getPosition().y + ray->getMarginTop() };
                        
                        Vector2D positionA = Vector2D { .x = a->getPosition().x + a->getMarginLeft(), .y = a->getPosition().y + a->getMarginTop() };
                        Vector2D positionB = Vector2D { .x = b->getPosition().x + b->getMarginLeft(), .y = b->getPosition().y + b->getMarginTop() };
        
                        distanceALeft = rayLeft - positionA;
                        distanceARight = rayRight - positionA;
                        distanceBLeft = rayLeft - positionB;
                        distanceBRight = rayRight - positionB;
                        
                        distanceALeft.x = fmax(distanceALeft.x, -distanceALeft.x);
                        distanceALeft.y = fmax(distanceALeft.y, -distanceALeft.y);
                        distanceARight.x = fmax(distanceARight.x, -distanceARight.x);
                        distanceARight.y = fmax(distanceARight.y, -distanceARight.y);
                        
                        Vector2D nearestARight = (distanceALeft.x + distanceALeft.y) < (distanceARight.x + distanceARight.y) ? distanceALeft : distanceARight;
                        
                        distanceBLeft.x = fmax(distanceBLeft.x, -distanceBLeft.x);
                        distanceBLeft.y = fmax(distanceBLeft.y, -distanceBLeft.y);
                        distanceBRight.x = fmax(distanceBRight.x, -distanceBRight.x);
                        distanceBRight.y = fmax(distanceBRight.y, -distanceBRight.y);
                        
                        Vector2D nearestB = distanceBLeft.x + distanceBLeft.y < distanceBRight.x + distanceBRight.y ? distanceBLeft : distanceBRight;
                        
                        return (nearestARight.x + nearestARight.y) < (nearestB.x + nearestB.y);
                        
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
