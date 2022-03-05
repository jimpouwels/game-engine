#include "scrollingWorld.hpp"
#include "gameEngine.hpp"
#include <math.h>

namespace jimp {

ScrollingWorld::ScrollingWorld(AnimatedGraphic* mainCharacter, int width, int height) {
    this->mainCharacter = mainCharacter;
}

ScrollingWorld::~ScrollingWorld() {
}

void ScrollingWorld::doOnUpdate() {
    GameEngine* gameEngine = GameEngine::getInstance();
    float rightSideOfCamera = gameEngine->getScreenWidth() / 2 + 125;
    float leftSideOfCamera = gameEngine->getScreenWidth() / 2 - 125;
    int offsetDeltaX = 0;
    if ((mainCharacter->getPosition().x + mainCharacter->getWidth()) >= rightSideOfCamera) {
        offsetDeltaX += (mainCharacter->getPosition().x + mainCharacter->getWidth()) - rightSideOfCamera;
        mainCharacter->getPosition().x = rightSideOfCamera - mainCharacter->getWidth();
    } else if (offsetX != 0 && mainCharacter->getPosition().x <= leftSideOfCamera) {
        offsetDeltaX -= leftSideOfCamera - mainCharacter->getPosition().x;
        mainCharacter->getPosition().x = leftSideOfCamera;
    }
    offsetX += offsetDeltaX;
    for (const auto& scrollingGraphic: *GameEngine::getInstance()->getAllGraphics()) {
        if (scrollingGraphic == mainCharacter || !scrollingGraphic->isApplyScrolling()) {
            continue;
        }
        scrollingGraphic->getPosition().x -= offsetDeltaX;
        if (scrollingGraphic->isPositionedWithinScreen()) {
            scrollingGraphic->show();
        } else {
            scrollingGraphic->hide();
        }
    }
}

void ScrollingWorld::doOnFrame() {
}

}
