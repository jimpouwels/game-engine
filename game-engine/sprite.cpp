#include "sprite.hpp"
#include <iostream>
#include <stdio.h>

namespace jimp {

Sprite::Sprite(int x, int y, std::string filePath) {
    this->x = x;
    this->y = y;
    this->filePath = filePath;
}

int Sprite::getX() {
    return this->x;
}

void Sprite::setX(int x) {
    this->x = x;
}

int Sprite::getY() {
    return this->y;
}

void Sprite::setY(int y) {
    this->y = y;
}

std::string Sprite::getFilePath() {
    return this->filePath;
}

}
