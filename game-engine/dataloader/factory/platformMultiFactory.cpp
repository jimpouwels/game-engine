#include "platformMultiFactory.hpp"
#include "gameEngine.hpp"
#include <vector>
#include <iostream>

namespace jimp {

PlatformMultiFactory::PlatformMultiFactory(uint16_t rowCount, uint16_t columnCount, float blockSize, float scale, jimp::Vector2D position) {
    this->rowCount = rowCount;
    this->columnCount = columnCount;
    this->blockSize = blockSize;
    this->scale = scale;
    this->position = position;
}

void PlatformMultiFactory::render() {
    float size = blockSize * scale;
    for (int x = 0; x < columnCount; x++) {
        for (int y = 0; y < rowCount; y++) {
            std::string filePath = "";
            AnimatedGraphic* block = new AnimatedGraphic();
            if (x == 0 && y == 0) {
                filePath = cornerLeftTopFilePath;
                block->setCollidable(true);
            } else if (x == columnCount - 1 && y == 0) {
                filePath = cornerRightTopFilePath;
                block->setCollidable(true);
            } else if (y == 0) {
                filePath = middleTopFilePath;
                block->setCollidable(true);
            } else if (x == 0 && y < rowCount - 1) {
                filePath = middleLeftFilePath;
                block->setCollidable(true);
            } else if (x == columnCount - 1 && y < rowCount - 1) {
                filePath = middleRightFilePath;
                block->setCollidable(true);
            } else if (y == rowCount - 1 && x < columnCount - 1 && x > 0) {
                filePath = middleBottomFilePath;
                block->setCollidable(true);
            } else if (y == rowCount - 1 && x == columnCount - 1) {
                filePath = cornerRightBottomFilePath;
                block->setCollidable(true);
            } else if (y == rowCount - 1 && x == 0) {
                filePath = cornerLeftBottomFilePath;
                block->setCollidable(true);
            } else {
                filePath = centerFilePath;
                block->setCollidable(false);
            }
            block->setPosition(Vector2D::from(position.x + (x * size), position.y + (y * size)));
            block->setApplyScrolling(true);
            block->setRotationAngle(0.0f);
            block->setScale(scale);
            block->setApplyGravity(false);
            block->setZIndex(zIndex);
            block->addSprite("default", filePath);
            GameEngine::getInstance()->registerGraphic(block);
        }
    }
}

void PlatformMultiFactory::setCornerTopLeft(std::string cornerLeftTopFilePath) {
    this->cornerLeftTopFilePath = cornerLeftTopFilePath;
}

void PlatformMultiFactory::setCornerTopRight(std::string cornerRightTopFilePath) {
    this->cornerRightTopFilePath = cornerRightTopFilePath;
}

void PlatformMultiFactory::setCornerBottomLeft(std::string cornerLeftBottomFilePath) {
    this->cornerLeftBottomFilePath = cornerLeftBottomFilePath;
}

void PlatformMultiFactory::setCornerBottomRight(std::string cornerRightBottomFilePath) {
    this->cornerRightBottomFilePath = cornerRightBottomFilePath;
}

void PlatformMultiFactory::setMiddleTop(std::string middleTopFilePath) {
    this->middleTopFilePath = middleTopFilePath;
}

void PlatformMultiFactory::setBottomMiddle(std::string middleBottomFilePath) {
    this->middleBottomFilePath = middleBottomFilePath;
}

void PlatformMultiFactory::setRightMiddle(std::string middleRightFilePath) {
    this->middleRightFilePath = middleRightFilePath;
}

void PlatformMultiFactory::setLeftMiddle(std::string middleLeftFilePath) {
    this->middleLeftFilePath = middleLeftFilePath;
}

void PlatformMultiFactory::setCenter(std::string centerFilePath) {
    this->centerFilePath = centerFilePath;
}

float PlatformMultiFactory::getWidth() {
    return columnCount * (blockSize * scale);
}

float PlatformMultiFactory::getHeight() {
    return rowCount * (blockSize * scale);
}

jimp::Vector2D PlatformMultiFactory::getPosition() {
    return position;
}

void PlatformMultiFactory::setZIndex(int zIndex) {
    this->zIndex = zIndex;
}

}
