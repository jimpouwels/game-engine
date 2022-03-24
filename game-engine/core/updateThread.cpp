#include "updateThread.hpp"
#include "scrollingWorld.hpp"
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

void doLoop(std::function<void(float)> onUpdateCallback, std::function<void(AnimatedGraphic*)> onGraphicDeletedCallBack, std::vector<AnimatedGraphic*>* allGraphics, std::list<AnimatedGraphic*>* newGraphics, std::recursive_mutex* processingLock, std::mutex* graphicsDeleteLock, KeyboardHandler* keyboardHandler) {
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
                if (ScrollingWorld::getInstance() != nullptr) {
                    ScrollingWorld::getInstance()->doOnUpdate(elapsed.count());
                }
                
                keyboardHandler->handleAllEvents();
                
                for (const auto& graphic : *allGraphics) {
                    graphic->onUpdate(elapsed.count());
                }
                for (const auto& graphic : *allGraphics) {
                    if (!graphic->isCollidable() || graphic->isMarkedForDeletion()) {
                        continue;
                    }
                    std::vector<AnimatedGraphic*> graphicsToCheckCollision = std::vector<AnimatedGraphic*>();
                    for (const auto& graphicToCheckCollision : *allGraphics) {
                        if (graphicToCheckCollision->isCollidable() &&
                            graphicToCheckCollision != graphic &&
                            graphic->canCollideWith(graphicToCheckCollision, elapsed.count()) &&
                            !graphic->isMarkedForDeletion()) {
                            graphicsToCheckCollision.push_back(graphicToCheckCollision);
                        }
                    }
                    sortByDistance(graphic, graphicsToCheckCollision);
                    for (const auto& graphicToCheckCollision : graphicsToCheckCollision) {
                        graphic->checkCollisionRect(graphicToCheckCollision, elapsed.count());
                    }
                }
                
                std::list<AnimatedGraphic*> graphicsToDelete = std::list<AnimatedGraphic*>();
                for (const auto& graphic : *allGraphics) {
                    if (graphic->isMarkedForDeletion()) {
                        graphicsToDelete.push_back(graphic);
                    }
                }
                for (const auto& graphicToDelete: graphicsToDelete) {
                    graphicsDeleteLock->lock();
                    onGraphicDeletedCallBack(graphicToDelete);
                    graphicsDeleteLock->unlock();
                }
                onUpdateCallback(elapsed.count());
            }
        } else {
            break;
        }
        processingLock->unlock();
    }
}

UpdateThread::UpdateThread(std::function<void(float)> onUpdateCallback, std::function<void(AnimatedGraphic*)> onGraphicDeletedCallback, KeyboardHandler* keyboardHandler) {
    this->processingLock = new std::recursive_mutex();
    this->graphicsDeletionLock = new std::mutex();
    this->onUpdateCallback = onUpdateCallback;
    this->onGraphicDeletedCallback = onGraphicDeletedCallback;
    this->allGraphics = new std::vector<AnimatedGraphic*>;
    this->newGraphics = new std::list<AnimatedGraphic*>;
    this->keyboardHandler = keyboardHandler;
}

UpdateThread::~UpdateThread() {
    removeAllGraphics();
    delete allGraphics;
    updateThread->join();
    delete updateThread;
    delete processingLock;
    delete graphicsDeletionLock;
}

void UpdateThread::start() {
    auto onGraphicDeletedLambda = std::bind(&UpdateThread::onGraphicDeleted, this, std::placeholders::_1);
    auto onGraphicUpdateLambda = std::bind(&UpdateThread::onUpdate, this, std::placeholders::_1);
    this->updateThread = new std::thread(doLoop, onGraphicUpdateLambda, onGraphicDeletedLambda, allGraphics, newGraphics, processingLock, graphicsDeletionLock, keyboardHandler);
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
    return allGraphics;
}

void UpdateThread::pause() {
    processingLock->lock();
}

void UpdateThread::unpause() {
    processingLock->unlock();
}

void UpdateThread::registerGraphic(AnimatedGraphic* graphic) {
    processingLock->lock();
    allGraphics->push_back(graphic);
    std::sort(allGraphics->begin(), allGraphics->end(), [](AnimatedGraphic* a, AnimatedGraphic* b) {
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
    for (const auto& sprite: *allGraphics) {
        graphicsToDelete.push_back(sprite);
    }
    for (const auto& spriteToDelete: graphicsToDelete) {
        onGraphicDeleted(spriteToDelete);
    }
    allGraphics->clear();
    graphicsDeletionLock->unlock();
}

void UpdateThread::onGraphicDeleted(AnimatedGraphic* graphic) {
    onGraphicDeletedCallback(graphic);
    allGraphics->erase(std::remove(allGraphics->begin(), allGraphics->end(), graphic), allGraphics->end());
    graphic->lockForDeletion();
    delete graphic;
}

}
