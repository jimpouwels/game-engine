#include "sprite.h"
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

int Sprite::getY() {
    return this->y;
}

std::string Sprite::getFilePath() {
    return this->filePath;
}

}
