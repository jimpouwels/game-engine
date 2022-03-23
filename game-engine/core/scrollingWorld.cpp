#include "scrollingWorld.hpp"
#include "gameEngine.hpp"
#include "rectangle.hpp"
#include <math.h>

namespace jimp {

static ScrollingWorld* scrollingWorldInstance = nullptr;

ScrollingWorld::ScrollingWorld(int width, int height) {
    scrollingWorldInstance = this;
    this->width = width;
    this->height = height;
    
    GameEngine* gameEngine = GameEngine::getInstance();
    this->maxScrollX = abs(width - gameEngine->getScreenWidth());
    this->maxScrollY = abs(height - gameEngine->getScreenHeight());
    
    gameEngine->addKeyListener(this);
    rightSideOfCamera = gameEngine->getScreenWidth() / 2 + 250;
    leftSideOfCamera = gameEngine->getScreenWidth() / 2 - 250;
    topSideOfCamera = gameEngine->getScreenHeight() / 2 - 250;
    bottomSideOfCamera = gameEngine->getScreenHeight() / 2;
}

ScrollingWorld::~ScrollingWorld() {
}

int ScrollingWorld::getWidth() {
    return width;
}

int ScrollingWorld::getHeight() {
    return height;
}

AnimatedGraphic* ScrollingWorld::getMainCharacter() {
    return mainCharacter;
}

void ScrollingWorld::setMainCharacter(AnimatedGraphic* animatedGraphic) {
    mainCharacter = animatedGraphic;
}

void ScrollingWorld::doOnUpdate(float elapsedTime) {
    if (mainCharacter == nullptr) {
        return;
    }
    float offsetDeltaX = 0;
    float offsetDeltaY = 0;
    if (!GameEngine::getInstance()->isEditMode()) {
        if (offsetX < maxScrollX && (mainCharacter->getScreenPositionRight()) >= rightSideOfCamera && mainCharacter->getMoveVelocity().x >= 0) {
            offsetDeltaX += (mainCharacter->getPosition().x + mainCharacter->getWidth() - mainCharacter->getMarginRight()) - rightSideOfCamera;
            mainCharacter->getPosition().x = rightSideOfCamera - mainCharacter->getWidth() + mainCharacter->getMarginRight();
        } else if (offsetX > 0 && mainCharacter->getScreenPositionLeft() <= leftSideOfCamera && mainCharacter->getMoveVelocity().x <= 0) {
            offsetDeltaX -= leftSideOfCamera - (mainCharacter->getPosition().x + mainCharacter->getMarginLeft());
            mainCharacter->getPosition().x = leftSideOfCamera - mainCharacter->getMarginLeft();
        }
        if (offsetY < maxScrollY && (mainCharacter->getPosition().y + mainCharacter->getHeight() - mainCharacter->getMarginBottom()) >= bottomSideOfCamera && mainCharacter->getMoveVelocity().y >= 0) {
            offsetDeltaY += (mainCharacter->getPosition().y + mainCharacter->getHeight() - mainCharacter->getMarginBottom()) - bottomSideOfCamera;
            mainCharacter->getPosition().y = bottomSideOfCamera - mainCharacter->getHeight() + mainCharacter->getMarginBottom();
        } else if (offsetY > 0 && (mainCharacter->getPosition().y + mainCharacter->getMarginTop() <= topSideOfCamera) && mainCharacter->getMoveVelocity().y <= 0) {
            offsetDeltaY -= topSideOfCamera - (mainCharacter->getPosition().y + mainCharacter->getMarginTop());
            mainCharacter->getPosition().y = topSideOfCamera - mainCharacter->getMarginTop();
        }
    } else {
        if (editCameraRight) {
            offsetDeltaX += Timing::toValueForElapsedTime(1000, elapsedTime);
        } else if (editCameraLeft) {
            offsetDeltaX -= Timing::toValueForElapsedTime(1000, elapsedTime);
        }
        if (editCameraDown) {
            offsetDeltaY += Timing::toValueForElapsedTime(1000, elapsedTime);
        } else if (editCameraUp) {
            offsetDeltaY -= Timing::toValueForElapsedTime(1000, elapsedTime);
        }
    }
    
    if (offsetX + offsetDeltaX > maxScrollX) {
        offsetDeltaX = maxScrollX - offsetX;
    } else if (offsetX + offsetDeltaX < 0) {
        offsetDeltaX = -offsetX;
    } else if (offsetY + offsetDeltaY > maxScrollY) {
        offsetDeltaY = maxScrollY - offsetY;
    } else if (offsetY + offsetDeltaY < 0) {
        offsetDeltaY = -offsetY;
    }
    
    offsetX = abs(offsetX + offsetDeltaX);
    offsetY = abs(offsetY + offsetDeltaY);
}

Vector2D ScrollingWorld::getOffset() {
    return Vector2D::from(-offsetX, -offsetY);
}

void ScrollingWorld::onKeyboardLeft(jimp::KeyState keyState) {
    editCameraLeft = keyState == KeyState::PRESSED;
}

void ScrollingWorld::onKeyboardRight(jimp::KeyState keyState) {
    editCameraRight = keyState == KeyState::PRESSED;
}

void ScrollingWorld::onKeyboardUp(jimp::KeyState keyState) {
    editCameraUp = keyState == KeyState::PRESSED;
}

void ScrollingWorld::onKeyboardDown(jimp::KeyState keyState) {
    editCameraDown = keyState == KeyState::PRESSED;
}

void ScrollingWorld::doOnFrame() {
//    GameEngine::getInstance()->drawRectangle(500, 500, Vector2D { .x = static_cast<float>(GameEngine::getInstance()->getScreenWidth() / 2 - 250), .y = static_cast<float>(GameEngine::getInstance()->getScreenHeight() / 2 - 250) }, 0xFFFFFF);
}

ScrollingWorld* ScrollingWorld::getInstance() {
    return scrollingWorldInstance;
}

}
