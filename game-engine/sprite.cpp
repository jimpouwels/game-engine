#include "sprite.hpp"
#include <iostream>
#include <stdio.h>
#include <SFML/Graphics.hpp>

namespace jimp {

Sprite::Sprite(int x, int y, std::string filePath) {
    this->x = x;
    this->y = y;
    loadImage(filePath);
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

void Sprite::setPosition(int x, int y) {
    setX(x);
    setY(y);
}

void Sprite::loadImage(std::string filePath) {
    this->filePath = filePath;
    this->image = new sf::Image();
    image->loadFromFile(filePath);
}

sf::Image& Sprite::getImage() {
    return *this->image;
}

std::string Sprite::getFilePath() {
    return filePath;
}

}
