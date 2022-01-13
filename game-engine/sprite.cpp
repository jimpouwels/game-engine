#include "sprite.hpp"
#include <iostream>
#include <stdio.h>
#include <SFML/Graphics.hpp>

namespace jimp {

Sprite::Sprite(float x, float y, std::string filePath) {
    this->x = x;
    this->y = y;
    loadImage(filePath);
}

Sprite::~Sprite() {
    delete image;
}

float Sprite::getX() {
    return this->x;
}

void Sprite::setX(float x) {
    this->x = x;
}

float Sprite::getY() {
    return this->y;
}

void Sprite::setY(float y) {
    this->y = y;
}

void Sprite::setPosition(float x, float y) {
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
