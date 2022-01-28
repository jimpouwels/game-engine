#include "sprite.hpp"
#include <iostream>
#include <stdio.h>
#include <SFML/Graphics.hpp>

namespace jimp {

Sprite::Sprite(float x, float y, float scale, int rotationAngle, std::string filePath) {
    Sprite(x, y, scale, filePath);
    this->rotationAngle = rotationAngle;
}

Sprite::Sprite(float x, float y, float scale, std::string filePath) {
    this->x = x;
    this->y = y;
    this->scale = scale;
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

int Sprite::getWidth() {
    return image->getSize().x * scale;
}

int Sprite::getHeight() {
    return image->getSize().y * scale;
}

float Sprite::getRotationAngle() {
    return rotationAngle;
}

void Sprite::setRotationAngle(float rotationAngle) {
    this->rotationAngle = rotationAngle;
}

std::string Sprite::getFilePath() {
    return filePath;
}

float Sprite::getScale() {
    return scale;
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

}
