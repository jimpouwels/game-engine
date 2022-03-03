#include "scrollingWorld.hpp"
#include "gameEngine.hpp"
#include <math.h>

namespace mario {

ScrollingWorld::ScrollingWorld(jimp::AnimatedGraphic* mainCharacter, int width, int height) {
    this->mainCharacter = mainCharacter;
    this->scrollingGraphics = new std::list<jimp::AnimatedGraphic*>;
}

ScrollingWorld::~ScrollingWorld() {
}

void ScrollingWorld::addGraphic(jimp::AnimatedGraphic* animatedGraphic) {
    this->scrollingGraphics->push_back(animatedGraphic);
}

void ScrollingWorld::doOnUpdate() {
    jimp::GameEngine* gameEngine = jimp::GameEngine::getInstance();
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
    for (const auto& scrollingGraphic: *scrollingGraphics) {
        if (scrollingGraphic == mainCharacter) {
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
    jimp::GameEngine* gameEngine = jimp::GameEngine::getInstance();
    gameEngine->drawRectangle(250, 250, jimp::Vector2D { .x = static_cast<float>(gameEngine->getScreenWidth() / 2 - 125), .y = static_cast<float>(gameEngine->getScreenHeight() / 2 - 125) }, 0xF7F7D1);
}

}
