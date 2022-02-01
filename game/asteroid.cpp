#include <math.h>
#include "asteroid.hpp"
#include "screen.hpp"
#include "geo2D.hpp"

#include <iostream>

const int Asteroid::SPEED_IN_PIXELS_PER_SECOND = 500;
const int Asteroid::ROTATION_DEGREES_PER_SECOND = 30;

Asteroid::Asteroid(jimp::Screen* screen, float x, float y) : jimp::AnimatedSprite(screen, x, y, 0.1F, -1) {
    addSprite("default", "asteroid.png");
    setRotationAngle(90);
    directionAngle = 135;
}

Asteroid::~Asteroid() {
    
}

void Asteroid::update(float elapsedTime) {
    handleDirection(elapsedTime);
    handleMovement(elapsedTime);

    this->updateAnimation(elapsedTime);
}

void Asteroid::handleDirection(float elapsedTime) {
    if (isOutsideScreenAbove()) {
        if (directionAngle == 0 || directionAngle == 180) {
            directionAngle += 180;
        } else if (directionAngle < 90) {
            directionAngle += 90;
        } else {
            directionAngle -= 90;
        }
    } else if (isOutsideScreenBelow()) {
        if (directionAngle == 0 || directionAngle == 180) {
            directionAngle += 180;
        } else if (directionAngle > 180) {
            directionAngle += 90;
        } else {
            directionAngle -= 90;
        }
    }
    if (isOutsideScreenLeft()) {
        if (directionAngle == 270 || directionAngle == 90) {
            directionAngle += 180;
        } else if (directionAngle > 270) {
            directionAngle += 90;
        } else {
            directionAngle -= 90;
        }
    } else if (isOutsideScreenRight()) {
        if (directionAngle == 270 || directionAngle == 90) {
            directionAngle += 180;
        } else if (directionAngle < 90) {
            directionAngle -= 90;
        } else {
            directionAngle += 90;
        }
    }
    directionAngle = jimp::Geo2D::normalizeAngle(directionAngle);
}

void Asteroid::handleMovement(float elapsedTime) {
    float distance = (SPEED_IN_PIXELS_PER_SECOND) / (1.0F / elapsedTime);
    jimp::Point2D deltaPosition = jimp::Geo2D::deltaByAngle(directionAngle, distance);
    addToPosition(deltaPosition);

    float deltaDegrees = ROTATION_DEGREES_PER_SECOND / (1.0F / elapsedTime);
    setRotationAngle(getRotationAngle() + deltaDegrees);
}
