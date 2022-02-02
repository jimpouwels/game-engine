#include <math.h>
#include "asteroid.hpp"
#include "screen.hpp"
#include "geo2D.hpp"

#include <iostream>

const int Asteroid::SPEED_IN_PIXELS_PER_SECOND = 180;
const int Asteroid::ROTATION_DEGREES_PER_SECOND = 50;

Asteroid::Asteroid(jimp::Screen* screen, float x, float y) : jimp::AnimatedSprite(screen, x, y, 0.1F, -1) {
    addSprite("default", "asteroid.png");
    setRotationAngle(90);
    directionAngle = 135;
}

Asteroid::~Asteroid() {
    
}

void Asteroid::update(float elapsedTime) {
    updateDirection(elapsedTime);
    updateMovement(elapsedTime);
    updateRotation(elapsedTime);

    this->updateAnimation(elapsedTime);
}

void Asteroid::updateDirection(float elapsedTime) {
    if (isOutsideScreenAbove() || isOutsideScreenBelow()) {
        directionAngle = jimp::Geo2D::inverseAngleVertically(directionAngle);
    }
    if (isOutsideScreenLeft() || isOutsideScreenRight()) {
        directionAngle = jimp::Geo2D::inverseAngleHorizontally(directionAngle);
    }
}

void Asteroid::updateMovement(float elapsedTime) {
    float distance = (SPEED_IN_PIXELS_PER_SECOND) / (1.0F / elapsedTime);
    jimp::Point2D deltaPosition = jimp::Geo2D::deltaByAngle(directionAngle, distance);
    addToPosition(deltaPosition);
}

void Asteroid::updateRotation(float elapsedTime) {
    float deltaDegrees = ROTATION_DEGREES_PER_SECOND / (1.0F / elapsedTime);
    setRotationAngle(getRotationAngle() + deltaDegrees);
}
