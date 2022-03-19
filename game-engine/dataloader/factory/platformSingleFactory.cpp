#include "platformSingleFactory.hpp"
#include "gameEngine.hpp"
#include <vector>
#include <iostream>

namespace jimp {

PlatformSingleFactory::PlatformSingleFactory(uint16_t columnCount, float blockSize, float scale, float transparency, jimp::Vector2D position) {
    this->columnCount = columnCount;
    this->blockSize = blockSize;
    this->scale = scale;
    this->position = position;
    this->transparency = transparency;
}

void PlatformSingleFactory::render() {
    float size = blockSize * scale;
    for (int x = 0; x < columnCount; x++) {
        std::string filePath = "";
        AnimatedGraphic* block = new AnimatedGraphic();
        if (x == 0) {
            filePath = cornerLeftFilePath;
        } else if (x == columnCount - 1) {
            filePath = cornerRightFilePath;
        } else {
            filePath = middleFilePath;
        }
        block->setPosition(Vector2D::from(position.x + (x * size), position.y));
        block->setApplyScrolling(true);
        block->setRotationAngle(0.0f);
        block->setScale(scale);
        block->setApplyGravity(false);
        block->setZIndex(zIndex);
        block->setTransparency(transparency);
        block->setCollidable(true);
        block->addSprite("default", filePath);
        GameEngine::getInstance()->registerGraphic(block);
    }
}

void PlatformSingleFactory::setCornerLeft(std::string cornerLeftFilePath) {
    this->cornerLeftFilePath = cornerLeftFilePath;
}

void PlatformSingleFactory::setCornerRight(std::string cornerRightFilePath) {
    this->cornerRightFilePath = cornerRightFilePath;
}

void PlatformSingleFactory::setMiddle(std::string middleFilePath) {
    this->middleFilePath = middleFilePath;
}

float PlatformSingleFactory::getWidth() {
    return columnCount * (blockSize * scale);
}

float PlatformSingleFactory::getHeight() {
    return blockSize * scale;
}

jimp::Vector2D PlatformSingleFactory::getPosition() {
    return position;
}

void PlatformSingleFactory::setZIndex(int zIndex) {
    this->zIndex = zIndex;
}

}
