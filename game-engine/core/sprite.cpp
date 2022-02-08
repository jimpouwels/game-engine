#include "sprite.hpp"
#include <iostream>
#include <stdio.h>

namespace jimp {

Sprite::Sprite(jimp::GameEngine* gameEngine, float x, float y, float scale, int angle, std::string filePath) {
    Sprite(gameEngine, x, y, scale, filePath);
    this->angle = angle;
}

Sprite::Sprite(jimp::GameEngine* gameEngine, float x, float y, float scale, std::string filePath) {
    this->position.x = x;
    this->position.y = y;
    this->scale = scale;
    this->gameEngine = gameEngine;
    this->image = gameEngine->loadImage(filePath);
    this->rotationPoint.x = getWidth() / 2.0F;
    this->rotationPoint.y = getHeight() / 2.0F;
}

Sprite::~Sprite() {
    
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

void Sprite::setRotationPoint(Vector2D rotationPoint) {
    this->rotationPoint = rotationPoint;
}

Vector2D Sprite::getRotationPoint() {
    return this->rotationPoint;
}

void Sprite::setPosition(Vector2D position) {
    this->position = position;
}

Vector2D& Sprite::getPosition() {
    return position;
}

bool Sprite::isAtLeftEdgeOfScreen() {
    return position.x < 0;
}

bool Sprite::isAtRightEdgeOfScreen() {
    return position.x > gameEngine->getScreenWidth() - getWidth();
}

bool Sprite::isAtTopEdgeOfScreen() {
    return position.y < 0;
}

bool Sprite::isAtBottomEdgeOfScreen() {
    return position.y > gameEngine->getScreenHeight() - getHeight();
}

bool Sprite::isOutsideScreenTop() {
    return position.y < -getHeight();
}

bool Sprite::isOutsideScreenBottom() {
    return position.y > gameEngine->getScreenHeight();
}

bool Sprite::isOutsideScreenLeft() {
    return position.x < -getWidth();
}

bool Sprite::isOutsideScreenRight() {
    return position.x > gameEngine->getScreenWidth();
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
