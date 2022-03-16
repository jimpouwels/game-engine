#include "updateThread.hpp"
#include "scrollingWorld.hpp"
#include "timer.hpp"
#include <iostream>
#include <chrono>

namespace jimp {

bool isWorking = false;
AnimatedGraphic* sourceGraphic = nullptr;

static void sortByDistance(std::vector<AnimatedGraphic *> &graphicsToCheckCollision) {
    std::sort(graphicsToCheckCollision.begin(), graphicsToCheckCollision.end(), [](AnimatedGraphic* a, AnimatedGraphic* b) {
        Vector2D sourceGraphicLeftTop = Vector2D::from(sourceGraphic->getScreenPositionLeft(), sourceGraphic->getScreenPositionTop());
        Vector2D sourceGraphicRightTop = Vector2D::from(sourceGraphic->getScreenPositionRight(), sourceGraphic->getScreenPositionTop());
        
        Vector2D positionA = Vector2D::from(a->getScreenPositionLeft(), a->getScreenPositionTop());
        Vector2D positionB = Vector2D::from(b->getScreenPositionLeft(), b->getScreenPositionTop());
        
        Vector2D distanceALeft = sourceGraphicLeftTop - positionA;
        Vector2D distanceARight = sourceGraphicRightTop - positionA;
        Vector2D distanceBLeft = sourceGraphicLeftTop - positionB;
        Vector2D distanceBRight = sourceGraphicRightTop - positionB;
        
        distanceALeft.x = fmax(distanceALeft.x, -distanceALeft.x);
        distanceALeft.y = fmax(distanceALeft.y, -distanceALeft.y);
        distanceARight.x = fmax(distanceARight.x, -distanceARight.x);
        distanceARight.y = fmax(distanceARight.y, -distanceARight.y);
        
        Vector2D nearestA = distanceALeft < distanceARight ? distanceALeft : distanceARight;
        
        distanceBLeft.x = fmax(distanceBLeft.x, -distanceBLeft.x);
        distanceBLeft.y = fmax(distanceBLeft.y, -distanceBLeft.y);
        distanceBRight.x = fmax(distanceBRight.x, -distanceBRight.x);
        distanceBRight.y = fmax(distanceBRight.y, -distanceBRight.y);
        
        Vector2D nearestB = distanceBLeft < distanceBRight ? distanceBLeft : distanceBRight;
        
        return nearestA < nearestB;
        
    });
}

void doLoop(std::function<void(float)> onUpdateCallback, std::function<void(AnimatedGraphic*)> onSpriteDeletedCallback, std::vector<AnimatedGraphic*>* registeredSprites, std::list<AnimatedGraphic*>* newGraphics, std::recursive_mutex* processingLock, std::mutex* graphicsDeleteLock) {
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
                Timer::getInstance()->onUpdate(elapsed.count());
                
                std::list<AnimatedGraphic*> spritesToDelete = std::list<AnimatedGraphic*>();
                
                for (uint16_t i = 0; i < registeredSprites->size(); i++) {
                    AnimatedGraphic* registeredSprite = registeredSprites->at(i);
                    if (!registeredSprite->isCollidable()) {
                        continue;
                    }
                    std::vector<AnimatedGraphic*> graphicsToCheckCollision = std::vector<AnimatedGraphic*>();
                    for (uint16_t x = 0; x < registeredSprites->size(); x++) {
                        AnimatedGraphic* graphicToCheckCollision = registeredSprites->at(x);
                        if (graphicToCheckCollision->isCollidable() && graphicToCheckCollision != registeredSprite && registeredSprite->canCollideWith(graphicToCheckCollision, elapsed.count())) {
                            graphicsToCheckCollision.push_back(graphicToCheckCollision);
                        }
                    }
                    sourceGraphic = registeredSprite;
                    sortByDistance(graphicsToCheckCollision);
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
                    graphicsDeleteLock->lock();
                    onSpriteDeletedCallback(spriteToDelete);
                    graphicsDeleteLock->unlock();
                }
                if (ScrollingWorld::getInstance() != nullptr) {
                    ScrollingWorld::getInstance()->doOnUpdate(elapsed.count());
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
    this->processingLock = new std::recursive_mutex();
    this->graphicsDeletionLock = new std::mutex();
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
    this->updateThread = new std::thread(doLoop, onGraphicUpdateLambda, onGraphicDeletedLambda, registeredGraphics, newGraphics, processingLock, graphicsDeletionLock);
}

void UpdateThread::stop() {
    processingLock->lock();
    isWorking = false;
    processingLock->unlock();
}

void UpdateThread::lockDeletionOfGraphics() {
    graphicsDeletionLock->lock();
}

void UpdateThread::unlockDeletionOfGraphics() {
    graphicsDeletionLock->unlock();
}

std::vector<AnimatedGraphic*>* UpdateThread::getAllGraphics() {
    return registeredGraphics;
}

void UpdateThread::pause() {
    processingLock->lock();
}

void UpdateThread::unpause() {
    processingLock->unlock();
}

void UpdateThread::registerGraphic(AnimatedGraphic* graphic) {
    processingLock->lock();
    registeredGraphics->push_back(graphic);
    std::sort(registeredGraphics->begin(), registeredGraphics->end(), [](AnimatedGraphic* a, AnimatedGraphic* b) {
        return a->getZIndex() > b->getZIndex();
    });
    processingLock->unlock();
}

void UpdateThread::onUpdate(float elapsedTime) {
    this->onUpdateCallback(elapsedTime);
}

void UpdateThread::removeAllGraphics() {
    graphicsDeletionLock->lock();
    std::list<AnimatedGraphic*> graphicsToDelete = std::list<AnimatedGraphic*>();
    for (const auto& sprite: *registeredGraphics) {
        graphicsToDelete.push_back(sprite);
    }
    for (const auto& spriteToDelete: graphicsToDelete) {
        onGraphicDeleted(spriteToDelete);
    }
    registeredGraphics->clear();
    graphicsDeletionLock->unlock();
}

void UpdateThread::onGraphicDeleted(AnimatedGraphic* graphic) {
    onGraphicDeletedCallback(graphic);
    registeredGraphics->erase(std::remove(registeredGraphics->begin(), registeredGraphics->end(), graphic), registeredGraphics->end());
    delete graphic;
}

}
