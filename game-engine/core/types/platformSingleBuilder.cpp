#include "platformSingleBuilder.hpp"
#include "platformBlock.hpp"
#include "gameEngine.hpp"
#include <vector>
#include <iostream>

namespace jimp {

PlatformSingleBuilder::PlatformSingleBuilder(uint16_t columnCount, float blockSize, float scale, jimp::Vector2D position) {
    this->columnCount = columnCount;
    this->blockSize = blockSize;
    this->scale = scale;
    this->position = position;
}

void PlatformSingleBuilder::render() {
    float size = blockSize * scale;
    for (int x = 0; x < columnCount; x++) {
        if (x == 0) {
            GameEngine::getInstance()->registerGraphic(new PlatformBlock(jimp::Vector2D::from(position.x + (x * size), position.y), cornerLeftFilePath, 0, scale, zIndex));
        } else if (x == columnCount - 1) {
            GameEngine::getInstance()->registerGraphic(new PlatformBlock(jimp::Vector2D::from(position.x + (x * size), position.y), cornerRightFilePath, 0, scale, zIndex));
        } else {
            GameEngine::getInstance()->registerGraphic(new PlatformBlock(jimp::Vector2D::from(position.x + (x * size), position.y), middleFilePath, 0, scale, zIndex));
        }
    }
}

void PlatformSingleBuilder::setCornerLeft(std::string cornerLeftFilePath) {
    this->cornerLeftFilePath = cornerLeftFilePath;
}

void PlatformSingleBuilder::setCornerRight(std::string cornerRightFilePath) {
    this->cornerRightFilePath = cornerRightFilePath;
}

void PlatformSingleBuilder::setMiddle(std::string middleFilePath) {
    this->middleFilePath = middleFilePath;
}

float PlatformSingleBuilder::getWidth() {
    return columnCount * (blockSize * scale);
}

float PlatformSingleBuilder::getHeight() {
    return blockSize * scale;
}

jimp::Vector2D PlatformSingleBuilder::getPosition() {
    return position;
}

void PlatformSingleBuilder::setZIndex(int zIndex) {
    this->zIndex = zIndex;
}

}
