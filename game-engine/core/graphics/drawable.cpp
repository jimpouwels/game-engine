#include "drawable.hpp"
#include "vector2D.hpp"

namespace jimp {

Drawable::Drawable(float x, float y, float scale, float angle) {
    this->position = Vector2D { .x = x, .y = y };
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

float Drawable::getRotationAngle() {
    return angle;
}

void Drawable::setRotationAngle(float angle) {
    this->angle = angle;
}

void Drawable::setRotationPoint(Vector2D rotationPoint) {
    this->rotationPoint = rotationPoint;
}

Vector2D Drawable::getRotationPoint() {
    return Vector2D { .x = getWidth() / 2.0F, .y = getHeight() / 2.0F };
}

float Drawable::getScale() {
    return scale;
}

void Drawable::setScale(float scale) {
    this->scale = scale;
}

}
