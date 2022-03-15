#include "drawable.hpp"
#include "vector2D.hpp"
#include "gameEngine.hpp"

namespace jimp {

Drawable::Drawable(float x, float y, float scale, float angle, uint16_t repeat) {
    this->position = Vector2D::from(x, y);
    this->repeat = repeat;
}

Drawable::~Drawable() {
}

Vector2D& Drawable::getPosition() {
    return position;
}

void Drawable::setPosition(Vector2D position) {
    this->position = position;
}

void Drawable::setX(float x) {
    position.x = x;
}

void Drawable::setY(float y) {
    position.y = y;
}

uint16_t Drawable::getRepeat() {
    return repeat;
}

float Drawable::getRotationAngle() {
    return angle;
}

void Drawable::setRotationAngle(float angle) {
    this->angle = angle;
}

void Drawable::setRotationPoint(Vector2D rotationPoint) {
    this->rotationPoint = rotationPoint;
}

void Drawable::setOffset(Vector2D offset) {
    this->offset = offset;
}

Vector2D Drawable::getOffset() {
    return this->offset;
}

bool Drawable::isPositionedWithinScreen() {
    if (getPosition().x + offset.x + getWidth() < 0) {
        return false;
    }
    if (getPosition().x + offset.x > GameEngine::getInstance()->getScreenWidth()) {
        return false;
    }
    if (getPosition().y + offset.y + getHeight() < 0) {
        return false;
    }
    if (getPosition().y + offset.y > GameEngine::getInstance()->getScreenHeight()) {
        return false;
    }
    return true;
}

Vector2D Drawable::getRotationPoint() {
    return Vector2D::from(getWidth() / 2.0F, getHeight() / 2.0F);
}

float Drawable::getScale() {
    return scale;
}

void Drawable::setScale(float scale) {
    this->scale = scale;
}

}
