#include "sprite.hpp"
#include <iostream>
#include <stdio.h>
#include <SFML/Graphics.hpp>

namespace jimp {

Sprite::Sprite(jimp::Screen* screen, float x, float y, float scale, int rotationAngle, std::string filePath) {
    Sprite(screen, x, y, scale, filePath);
    this->rotationAngle = rotationAngle;
}

Sprite::Sprite(jimp::Screen* screen, float x, float y, float scale, std::string filePath) {
    this->x = x;
    this->y = y;
    this->scale = scale;
    this->screen = screen;
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
    if (this->rotationAngle >= 360) {
        this->rotationAngle -= 360;
    } else if (this->rotationAngle <= 0) {
        this->rotationAngle += 360;
    }
}

bool Sprite::isOutsideScreenLeft() {
    return this->x < 0;
}

bool Sprite::isOutsideScreenRight() {
    return getX() > screen->getWidth() - getWidth();
}

bool Sprite::isOutsideScreenAbove() {
    return this->y < 0;
}

bool Sprite::isOutsideScreenBelow() {
    return getY() > screen->getHeight() - getHeight();
}

std::string Sprite::getFilePath() {
    return filePath;
}

float Sprite::getScale() {
    return scale;
}

void Sprite::setScale(float scale) {
    this->scale = scale;
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
