#include "scrollingWorld.hpp"
#include "gameEngine.hpp"
#include "rectangle.hpp"
#include <math.h>

namespace jimp {

static ScrollingWorld* scrollingWorldInstance = nullptr;

ScrollingWorld::ScrollingWorld(AnimatedGraphic* mainCharacter, int width, int height) {
    scrollingWorldInstance = this;
    this->width = width;
    this->height = height;
    this->maxScrollX = width - GameEngine::getInstance()->getScreenWidth();
    this->maxScrollY = height - GameEngine::getInstance()->getScreenHeight();
    this->mainCharacter = mainCharacter;
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

void ScrollingWorld::setMainCharacterLoaded(bool loaded) {
    mainCharacterLoaded = loaded;
}

void ScrollingWorld::doOnUpdate() {
    if (!mainCharacterLoaded) {
        return;
    }
    GameEngine* gameEngine = GameEngine::getInstance();
    float rightSideOfCamera = gameEngine->getScreenWidth() / 2 + 125;
    float leftSideOfCamera = gameEngine->getScreenWidth() / 2 - 125;
    float topSideOfCamera = gameEngine->getScreenHeight() / 2 - 250;
    float bottomSideOfCamera = gameEngine->getScreenHeight() / 2;
    float offsetDeltaX = 0;
    float offsetDeltaY = 0;
    if (offsetX < maxScrollX && (mainCharacter->getPosition().x + mainCharacter->getWidth() - mainCharacter->getMarginRight()) >= rightSideOfCamera && mainCharacter->getVelocity().x > 0) {
        offsetDeltaX += (mainCharacter->getPosition().x + mainCharacter->getWidth() - mainCharacter->getMarginRight()) - rightSideOfCamera;
        if (offsetX + offsetDeltaX > maxScrollX) {
            offsetDeltaX = maxScrollX - offsetX;
        }
        mainCharacter->getPosition().x = rightSideOfCamera - mainCharacter->getWidth() + mainCharacter->getMarginRight();
    } else if (offsetX > 0 && mainCharacter->getPosition().x + mainCharacter->getMarginLeft() <= leftSideOfCamera && mainCharacter->getVelocity().x < 0) {
        offsetDeltaX -= leftSideOfCamera - mainCharacter->getPosition().x + mainCharacter->getMarginLeft();
        if (offsetX - offsetDeltaX < 0) {
            offsetDeltaX = -offsetX;
        }
        mainCharacter->getPosition().x = leftSideOfCamera - mainCharacter->getMarginLeft();
    }
    if (offsetY < maxScrollY && (mainCharacter->getPosition().y + mainCharacter->getHeight() - mainCharacter->getMarginBottom()) >= bottomSideOfCamera && mainCharacter->getVelocity().y > 0) {
        offsetDeltaY += (mainCharacter->getPosition().y + mainCharacter->getHeight() - mainCharacter->getMarginBottom()) - bottomSideOfCamera;
        if (offsetY + offsetDeltaY > maxScrollY) {
            offsetDeltaY = maxScrollY - offsetY;
        }
        mainCharacter->getPosition().y = bottomSideOfCamera - mainCharacter->getHeight() + mainCharacter->getMarginBottom();
    } else if (offsetY > 0 && (mainCharacter->getPosition().y + mainCharacter->getMarginTop() <= topSideOfCamera) && mainCharacter->getVelocity().y < 0) {
        offsetDeltaY -= topSideOfCamera - mainCharacter->getPosition().y + mainCharacter->getMarginTop();
        if (offsetY - offsetDeltaY < 0) {
            offsetDeltaY = -offsetY;
        }
        mainCharacter->getPosition().y = topSideOfCamera - mainCharacter->getMarginTop();
    }
    offsetX += offsetDeltaX;
    offsetY += offsetDeltaY;
    for (const auto& scrollingGraphic: *GameEngine::getInstance()->getAllGraphics()) {
        if (scrollingGraphic == mainCharacter || !scrollingGraphic->isApplyScrolling()) {
            continue;
        }
        scrollingGraphic->getPosition().x -= offsetDeltaX;
        scrollingGraphic->getPosition().y -= offsetDeltaY;
        if (scrollingGraphic->isPositionedWithinScreen()) {
            scrollingGraphic->show();
        } else {
            scrollingGraphic->hide();
        }
    }
}

void ScrollingWorld::doOnFrame() {
}

ScrollingWorld* ScrollingWorld::getInstance() {
    return scrollingWorldInstance;
}

}
