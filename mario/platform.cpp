#include "platform.hpp"
#include "platformBlock.hpp"
#include <vector>
#include <iostream>

namespace ghosty {

Platform::Platform(std::string cornerBlockFilePath, std::string innerBlockFilePath, uint16_t rowCount, uint16_t columnCount, float blockSize, float scale, jimp::Vector2D position) {
    this->rowCount = rowCount;
    this->columnCount = columnCount;
    this->blockSize = blockSize;
    this->scale = scale;
    this->position = position;
    float size = blockSize * scale;
    if (rowCount == 1) {
        for (int x = 0; x < columnCount; x++) {
            if (x == 0) {
                PlatformBlock* left = new PlatformBlock(jimp::Vector2D { .x = position.x + (x * size), .y = position.y }, "platform-single-corner-left.png", 0, scale);
            } else if (x == columnCount - 1) {
                PlatformBlock* right = new PlatformBlock(jimp::Vector2D { .x = position.x + (x * size), .y = position.y }, "platform-single-corner-right.png", 0, scale);
            } else {
                PlatformBlock* middle = new PlatformBlock(jimp::Vector2D { .x = position.x + (x * size), .y = position.y }, "platform-single-middle.png", 0, scale);
            }
        }
        return;
    }
    for (int x = 0; x < columnCount; x++) {
        for (int y = 0; y < rowCount; y++) {
            if (x == 0 && y == 0) {
                PlatformBlock* leftTop = new PlatformBlock(jimp::Vector2D { .x = position.x + (x * size), .y = position.y + (y * size) }, "platform-top-corner-left.png", 0, scale);
            } else if (x == columnCount - 1 && y == 0) {
                PlatformBlock* rightTop = new PlatformBlock(jimp::Vector2D { .x = position.x + (x * size), .y = position.y + (y * size) }, "platform-top-corner-right.png", 0, scale);
            } else if (y == 0) {
                PlatformBlock* topMiddle = new PlatformBlock(jimp::Vector2D { .x = position.x + (x * size), .y = position.y + (y * size) }, "platform-top-middle.png", 0, scale);
            } else if (x == 0 && y < rowCount - 1) {
                PlatformBlock* leftMiddle = new PlatformBlock(jimp::Vector2D { .x = position.x + (x * size), .y = position.y + (y * size) }, "platform-left-middle.png", 0, scale);
            } else if (x == columnCount - 1 && y < rowCount - 1) {
                PlatformBlock* rightMiddle = new PlatformBlock(jimp::Vector2D { .x = position.x + (x * size), .y = position.y + (y * size) }, "platform-right-middle.png", 0, scale);
            } else if (y == rowCount - 1 && x < columnCount - 1 && x > 0) {
                PlatformBlock* bottomMiddle = new PlatformBlock(jimp::Vector2D { .x = position.x + (x * size), .y = position.y + (y * size) }, "platform-bottom-middle.png", 0, scale);
            } else if (y == rowCount - 1 && x == columnCount - 1) {
                PlatformBlock* rightBottom = new PlatformBlock(jimp::Vector2D { .x = position.x + (x * size), .y = position.y + (y * size) }, "platform-bottom-corner-right.png", 0, scale);
                rightBottom->drawInversedHorizontally(true);
            } else if (y == rowCount - 1 && x == 0) {
                PlatformBlock* leftBottom = new PlatformBlock(jimp::Vector2D { .x = position.x + (x * size), .y = position.y + (y * size) }, "platform-bottom-corner-left.png", 0, scale);
            } else {
                PlatformBlock* center = new PlatformBlock(jimp::Vector2D { .x = position.x + (x * size), .y = position.y + (y * size) }, "platform-center.png", 0, scale);
                center->setCollidable(false);
            }
        }
    }
}

float Platform::getWidth() {
    return columnCount * (blockSize * scale);
}

float Platform::getHeight() {
    return rowCount * (blockSize * scale);
}

jimp::Vector2D Platform::getPosition() {
    return position;
}

}
