#include "scrollingWorld.hpp"
#include "gameEngine.hpp"
#include <math.h>

namespace mario {

ScrollingWorld::ScrollingWorld(jimp::AnimatedGraphic* mainCharacter, int width, int height) {
    this->mainCharacter = mainCharacter;
}

ScrollingWorld::~ScrollingWorld() {
}

void ScrollingWorld::doOnUpdate() {
    jimp::GameEngine* gameEngine = jimp::GameEngine::getInstance();
    float rightSideOfCamera = gameEngine->getScreenWidth() / 2 + 125;
    float leftSideOfCamera = gameEngine->getScreenWidth() / 2 - 125;
    int offsetDeltaX = 0;
    if ((mainCharacter->getPosition().x + mainCharacter->getWidth()) > rightSideOfCamera) {
        offsetDeltaX += (mainCharacter->getPosition().x + mainCharacter->getWidth()) - rightSideOfCamera;
        mainCharacter->getPosition().x = rightSideOfCamera - mainCharacter->getWidth();
    } else if (mainCharacter->getPosition().x < leftSideOfCamera) {
        offsetDeltaX -= leftSideOfCamera - mainCharacter->getPosition().x;
        mainCharacter->getPosition().x = leftSideOfCamera;
    }
    offsetX += offsetDeltaX;
    std::vector<jimp::AnimatedGraphic*>* graphics = gameEngine->getAllGraphics();
    for (int i = 0; i < graphics->size(); i++) {
        jimp::AnimatedGraphic* graphic = graphics->at(i);
        if (graphic == mainCharacter) {
            continue;
        }
        graphic->getPosition().x -= offsetDeltaX;
        if (graphic->isPositionedWithinScreen()) {
            graphic->show();
        } else {
            std::cout << rand() << "HIDE IT" << std::endl;
            graphic->hide();
        }
    }
}

void ScrollingWorld::doOnFrame() {
    jimp::GameEngine* gameEngine = jimp::GameEngine::getInstance();
    gameEngine->drawRectangle(250, 250, jimp::Vector2D { .x = static_cast<float>(gameEngine->getScreenWidth() / 2 - 125), .y = static_cast<float>(gameEngine->getScreenHeight() / 2 - 125) }, 0xEBB7A9);
}

}
