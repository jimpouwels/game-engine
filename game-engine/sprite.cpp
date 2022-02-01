#include "sprite.hpp"
#include <iostream>
#include <stdio.h>
#include <SFML/Graphics.hpp>

namespace jimp {

Sprite::Sprite(jimp::Screen* screen, float x, float y, float scale, int angle, std::string filePath) {
    Sprite(screen, x, y, scale, filePath);
    this->angle = angle;
}

Sprite::Sprite(jimp::Screen* screen, float x, float y, float scale, std::string filePath) {
    this->position.x = x;
    this->position.y = y;
    this->scale = scale;
    this->screen = screen;
    loadImage(filePath);
}

Sprite::~Sprite() {
    delete image;
}

void Sprite::setX(float x) {
    position.x = x;
}

void Sprite::setY(float y) {
    position.y = y;
}

int Sprite::getWidth() {
    return image->getSize().x * scale;
}

int Sprite::getHeight() {
    return image->getSize().y * scale;
}

float Sprite::getRotationAngle() {
    return angle;
}

void Sprite::setPosition(Point2D position) {
    this->position = position;
}

Point2D& Sprite::getPosition() {
    return position;
}

void Sprite::setRotationAngle(float angle) {
    this->angle = angle;
}

bool Sprite::isOutsideScreenLeft() {
    return position.x < 0;
}

bool Sprite::isOutsideScreenRight() {
    return position.x > screen->getWidth() - getWidth();
}

bool Sprite::isOutsideScreenAbove() {
    return position.y < 0;
}

bool Sprite::isOutsideScreenBelow() {
    return position.y > screen->getHeight() - getHeight();
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
