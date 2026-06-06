#include "updateThread.hpp"
#include "gameEngine.hpp"
#include "scroller.hpp"
#include "timer.hpp"
#include <iostream>
#include <chrono>

namespace jimp {

bool isWorking = false;

static void sortByDistance(AnimatedGraphic* sourceGraphic, std::vector<AnimatedGraphic *> &graphicsToCheckCollision) {
    std::sort(graphicsToCheckCollision.begin(), graphicsToCheckCollision.end(), [&sourceGraphic](AnimatedGraphic* a, AnimatedGraphic* b) {
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

void doLoop(std::function<void(float)> onUpdateCallback, std::function<void(AnimatedGraphic*)> onGraphicDeletedCallBack, GameEngine* gameEngine, std::recursive_mutex* processingLock, KeyboardHandler* keyboardHandler) {
    isWorking = true;
    std::chrono::time_point<std::chrono::system_clock> previousUpdateTime;
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
        processingLock->lock();
        if (isWorking) {
            std::chrono::time_point<std::chrono::system_clock> currentTime = std::chrono::system_clock::now();
            std::chrono::duration<float> elapsed = currentTime - previousUpdateTime;
            previousUpdateTime = currentTime;
            if (elapsed.count() < 1) {
                Timer::getInstance()->onUpdate(elapsed.count());
                if (Scroller::getInstance() != nullptr) {
                    Scroller::getInstance()->doOnUpdate(elapsed.count());
                }
                
                keyboardHandler->handleAllEvents();
                
                std::list<AnimatedGraphic*> graphicsToDelete;
                {
                    auto locked = gameEngine->getAllGraphics();
                    for (const auto& graphic : locked.graphics) {
                        graphic->onUpdate(elapsed.count());
                    }
                    for (const auto& graphic : locked.graphics) {
                        if (!graphic->isCollidable() || graphic->isMarkedForDeletion()) {
                            continue;
                        }
                        std::vector<AnimatedGraphic*> graphicsToCheckCollision;
                        for (const auto& graphicToCheckCollision : locked.graphics) {
                            if (graphicToCheckCollision->isCollidable() &&
                                graphicToCheckCollision.get() != graphic.get() &&
                                graphic->canCollideWith(graphicToCheckCollision.get(), elapsed.count()) &&
                                !graphic->isMarkedForDeletion()) {
                                graphicsToCheckCollision.push_back(graphicToCheckCollision.get());
                            }
                        }
                        sortByDistance(graphic.get(), graphicsToCheckCollision);
                        for (AnimatedGraphic* graphicToCheckCollision : graphicsToCheckCollision) {
                            graphic->checkCollisionRect(graphicToCheckCollision, elapsed.count());
                        }
                    }
                    for (const auto& graphic : locked.graphics) {
                        if (graphic->isMarkedForDeletion()) {
                            graphicsToDelete.push_back(graphic.get());
                        }
                    }
                }
                for (AnimatedGraphic* graphicToDelete : graphicsToDelete) {
                    onGraphicDeletedCallBack(graphicToDelete);
                }
                onUpdateCallback(elapsed.count());
            }
        } else {
            break;
        }
        processingLock->unlock();
    }
}

UpdateThread::UpdateThread(std::function<void(float)> onUpdateCallback, std::function<void(AnimatedGraphic*)> onGraphicDeletedCallback, GameEngine* gameEngine, KeyboardHandler* keyboardHandler) {
    this->processingLock = new std::recursive_mutex();
    this->onUpdateCallback = onUpdateCallback;
    this->onGraphicDeletedCallback = onGraphicDeletedCallback;
    this->gameEngine = gameEngine;
    this->keyboardHandler = keyboardHandler;
}

UpdateThread::~UpdateThread() {
    updateThread->join();
    delete updateThread;
    delete processingLock;
}

void UpdateThread::start() {
    auto onGraphicDeletedLambda = std::bind(&UpdateThread::onGraphicDeleted, this, std::placeholders::_1);
    auto onGraphicUpdateLambda = std::bind(&UpdateThread::onUpdate, this, std::placeholders::_1);
    this->updateThread = new std::thread(doLoop, onGraphicUpdateLambda, onGraphicDeletedLambda, gameEngine, processingLock, keyboardHandler);
}

void UpdateThread::stop() {
    processingLock->lock();
    isWorking = false;
    processingLock->unlock();
}

void UpdateThread::pause() {
    processingLock->lock();
}

void UpdateThread::unpause() {
    processingLock->unlock();
}


void UpdateThread::onUpdate(float elapsedTime) {
    this->onUpdateCallback(elapsedTime);
}

void UpdateThread::removeAllGraphics() {
    std::list<AnimatedGraphic*> graphicsToDelete;
    {
        auto locked = gameEngine->getAllGraphics();
        for (const auto& sprite : locked.graphics) {
            graphicsToDelete.push_back(sprite.get());
        }
    }
    for (AnimatedGraphic* spriteToDelete : graphicsToDelete) {
        onGraphicDeleted(spriteToDelete);
    }
}

void UpdateThread::onGraphicDeleted(AnimatedGraphic* graphic) {
    onGraphicDeletedCallback(graphic);
}

}
