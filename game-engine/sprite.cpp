#include "sprite.hpp"
#include <iostream>
#include <stdio.h>

namespace jimp {

Sprite::Sprite(jimp::GamingInterface* gamingInterface, float x, float y, float scale, int angle, std::string filePath) {
    Sprite(gamingInterface, x, y, scale, filePath);
    this->angle = angle;
}

Sprite::Sprite(jimp::GamingInterface* gamingInterface, float x, float y, float scale, std::string filePath) {
    this->position.x = x;
    this->position.y = y;
    this->scale = scale;
    this->gamingInterface = gamingInterface;
    this->image = gamingInterface->loadImage(filePath);
}

Sprite::~Sprite() {
    // delete shared pointer?
}

void Sprite::setX(float x) {
    position.x = x;
}

void Sprite::setY(float y) {
    position.y = y;
}

int Sprite::getWidth() {
    return image->getWidth() * scale;
}

int Sprite::getHeight() {
    return image->getHeight() * scale;
}

float Sprite::getRotationAngle() {
    return angle;
}

void Sprite::setRotationAngle(float angle) {
    this->angle = angle;
}

void Sprite::setPosition(Vector2D position) {
    this->position = position;
}

Vector2D& Sprite::getPosition() {
    return position;
}

bool Sprite::isOutsideScreenLeft() {
    return position.x < 0;
}

bool Sprite::isOutsideScreenRight() {
    return position.x > gamingInterface->getScreen()->getWidth() - getWidth();
}

bool Sprite::isOutsideScreenAbove() {
    return position.y < 0;
}

bool Sprite::isOutsideScreenBelow() {
    return position.y > gamingInterface->getScreen()->getHeight() - getHeight();
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

jimp::Image& Sprite::getImage() {
    return *this->image;
}

}
