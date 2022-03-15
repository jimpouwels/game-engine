#include "platformMultiBuilder.hpp"
#include "platformBlock.hpp"
#include "gameEngine.hpp"
#include <vector>
#include <iostream>

namespace jimp {

PlatformMultiBuilder::PlatformMultiBuilder(uint16_t rowCount, uint16_t columnCount, float blockSize, float scale, jimp::Vector2D position) {
    this->rowCount = rowCount;
    this->columnCount = columnCount;
    this->blockSize = blockSize;
    this->scale = scale;
    this->position = position;
}

void PlatformMultiBuilder::render() {
    float size = blockSize * scale;
    for (int x = 0; x < columnCount; x++) {
        for (int y = 0; y < rowCount; y++) {
            if (x == 0 && y == 0) {
                GameEngine::getInstance()->registerGraphic(new PlatformBlock(jimp::Vector2D::from(position.x + (x * size), position.y + (y * size)), cornerLeftTopFilePath, 0, scale, zIndex));
            } else if (x == columnCount - 1 && y == 0) {
                GameEngine::getInstance()->registerGraphic(new PlatformBlock(jimp::Vector2D::from(position.x + (x * size), position.y + (y * size)), cornerRightTopFilePath, 0, scale, zIndex));
            } else if (y == 0) {
                GameEngine::getInstance()->registerGraphic(new PlatformBlock(jimp::Vector2D::from(position.x + (x * size), position.y + (y * size)), middleTopFilePath, 0, scale, zIndex));
            } else if (x == 0 && y < rowCount - 1) {
                GameEngine::getInstance()->registerGraphic(new PlatformBlock(jimp::Vector2D::from(position.x + (x * size), position.y + (y * size)), middleLeftFilePath, 0, scale, zIndex));
            } else if (x == columnCount - 1 && y < rowCount - 1) {
                GameEngine::getInstance()->registerGraphic(new PlatformBlock(jimp::Vector2D::from(position.x + (x * size), position.y + (y * size)), middleRightFilePath, 0, scale, zIndex));
            } else if (y == rowCount - 1 && x < columnCount - 1 && x > 0) {
                GameEngine::getInstance()->registerGraphic(new PlatformBlock(jimp::Vector2D::from(position.x + (x * size), position.y + (y * size)), middleBottomFilePath, 0, scale, zIndex));
            } else if (y == rowCount - 1 && x == columnCount - 1) {
                PlatformBlock* rightBottom = new PlatformBlock(jimp::Vector2D::from(position.x + (x * size), position.y + (y * size)), cornerRightBottomFilePath, 0, scale, zIndex);
                GameEngine::getInstance()->registerGraphic(rightBottom);
            } else if (y == rowCount - 1 && x == 0) {
                GameEngine::getInstance()->registerGraphic(new PlatformBlock(jimp::Vector2D::from(position.x + (x * size), position.y + (y * size)), cornerLeftBottomFilePath, 0, scale, zIndex));
            } else {
                PlatformBlock* center = new PlatformBlock(jimp::Vector2D::from(position.x + (x * size), position.y + (y * size)), centerFilePath, 0, scale, zIndex);
                GameEngine::getInstance()->registerGraphic(center);
                center->setCollidable(false);
            }
        }
    }
}

void PlatformMultiBuilder::setCornerTopLeft(std::string cornerLeftTopFilePath) {
    this->cornerLeftTopFilePath = cornerLeftTopFilePath;
}

void PlatformMultiBuilder::setCornerTopRight(std::string cornerRightTopFilePath) {
    this->cornerRightTopFilePath = cornerRightTopFilePath;
}

void PlatformMultiBuilder::setCornerBottomLeft(std::string cornerLeftBottomFilePath) {
    this->cornerLeftBottomFilePath = cornerLeftBottomFilePath;
}

void PlatformMultiBuilder::setCornerBottomRight(std::string cornerRightBottomFilePath) {
    this->cornerRightBottomFilePath = cornerRightBottomFilePath;
}

void PlatformMultiBuilder::setMiddleTop(std::string middleTopFilePath) {
    this->middleTopFilePath = middleTopFilePath;
}

void PlatformMultiBuilder::setBottomMiddle(std::string middleBottomFilePath) {
    this->middleBottomFilePath = middleBottomFilePath;
}

void PlatformMultiBuilder::setRightMiddle(std::string middleRightFilePath) {
    this->middleRightFilePath = middleRightFilePath;
}

void PlatformMultiBuilder::setLeftMiddle(std::string middleLeftFilePath) {
    this->middleLeftFilePath = middleLeftFilePath;
}

void PlatformMultiBuilder::setCenter(std::string centerFilePath) {
    this->centerFilePath = centerFilePath;
}

float PlatformMultiBuilder::getWidth() {
    return columnCount * (blockSize * scale);
}

float PlatformMultiBuilder::getHeight() {
    return rowCount * (blockSize * scale);
}

jimp::Vector2D PlatformMultiBuilder::getPosition() {
    return position;
}

void PlatformMultiBuilder::setZIndex(int zIndex) {
    this->zIndex = zIndex;
}

}
