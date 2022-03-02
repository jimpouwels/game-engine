#include "scrollingWorld.hpp"
#include "gameEngine.hpp"
#include <math.h>

namespace mario {

ScrollingWorld::ScrollingWorld(jimp::AnimatedGraphic* mainCharacter, int rows, int columns, uint16_t tileSize) {
    this->mainCharacter = mainCharacter;
    this->tileSize = tileSize;
    this->tileMap = new TileMap(rows, columns);
}

ScrollingWorld::~ScrollingWorld() {
    delete tileMap;
}

void ScrollingWorld::addGraphic(int x, int y, jimp::AnimatedGraphic* animatedGraphic) {
    tileMap->getTileAt(x, y)->add(animatedGraphic);
}

void ScrollingWorld::doOnUpdate() {
    jimp::GameEngine* gameEngine = jimp::GameEngine::getInstance();
    float rightSideOfCamera = gameEngine->getScreenWidth() / 2 + 125;
    float leftSideOfCamera = gameEngine->getScreenWidth() / 2 - 125;
    if (mainCharacter->getPosition().x >= rightSideOfCamera) {
        offsetX += mainCharacter->getPosition().x - rightSideOfCamera;
        mainCharacter->getPosition().x = rightSideOfCamera;
    } else if (mainCharacter->getPosition().x <= leftSideOfCamera) {
        offsetX -= leftSideOfCamera - mainCharacter->getPosition().x;
        mainCharacter->getPosition().x = leftSideOfCamera;
    }
    std::cout << offsetX << std::endl;
    for (int x = 0; x < tileMap->getColumnCount(); x++) {
        for (int y = 0; y < tileMap->getRowCount(); y++) {
            Tile* tile = tileMap->getTileAt(x, y);
            if (tile->getAnimatedGraphic() != nullptr) {
                jimp::AnimatedGraphic* graphic = tile->getAnimatedGraphic();
                graphic->getPosition().x = x * tileSize - offsetX;
                if (graphic->getPosition().x < -(graphic->getWidth())) {
                    graphic->hide();
                } else {
                    graphic->show();
                }
            }
        }
    }
}

void ScrollingWorld::doOnFrame() {
    jimp::GameEngine* gameEngine = jimp::GameEngine::getInstance();
    gameEngine->drawRectangle(250, 250, jimp::Vector2D { .x = static_cast<float>(gameEngine->getScreenWidth() / 2 - 125), .y = static_cast<float>(gameEngine->getScreenHeight() / 2 - 125) }, 0xEBB7A9);
}

}
