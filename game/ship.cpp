#include <math.h>

#include "ship.hpp"
#include "shipEventListener.hpp"
#include "bullet.hpp"
#include "direction.hpp"
#include "geometricsHelper.hpp"
#include "position2D.hpp"

const int Ship::SPEED_IN_PIXELS_PER_SECOND = 200;
const int Ship::ROTATION_DEGREES_PER_SECOND = 140;
const int Ship::SHOTS_PER_SECOND = 10;
const float Ship::SCALE = 0.2F;

Ship::Ship(jimp::Screen* screen, ShipEventListener* eventListener) : jimp::AnimatedSprite(screen, screen->getWidth() / 2, screen->getHeight() / 2, SCALE, -1) {
    this->eventListener = eventListener;
    addSprite("default", "spaceship.png");
}

void Ship::update(float elapsedTime) {
    handleFiring(elapsedTime);
    handleMovement(elapsedTime);
    handleRotation(elapsedTime);
}

void Ship::onKeyboardLeft(jimp::KeyState keyState) {
    isRotatingLeft = keyState == jimp::KeyState::PRESSED;
}

void Ship::onKeyboardRight(jimp::KeyState keyState) {
    isRotatingRight = keyState == jimp::KeyState::PRESSED;
}

void Ship::onKeyboardUp(jimp::KeyState keyState) {
    isThrothling = keyState == jimp::KeyState::PRESSED;
}
void Ship::onKeyboardDown(jimp::KeyState keyState) {
    isThrothling = keyState == jimp::KeyState::PRESSED;
}

void Ship::onKeyboardSpaceBar(jimp::KeyState keyState) {
    if (keyState == jimp::KeyState::PRESSED) {
        hasFired = keyState == jimp::KeyState::PRESSED;
    }
    isFiring = keyState == jimp::KeyState::PRESSED;
}

void Ship::handleFiring(float elapsedTime) {
    elapsedTimeSinceLastShot += elapsedTime;
    if (!isFiring && !hasFired) {
        return;
    }
    float timeBetweenShots = 1.0F / SHOTS_PER_SECOND;
    if (elapsedTimeSinceLastShot >= timeBetweenShots && (hasFired || isFiring)) {
        hasFired = false;
        Bullet* bullet = new Bullet(&getScreen(), getX() + (getWidth() / 2), getY() + (getHeight() / 2), getRotationAngle());
        eventListener->onWeaponFired(bullet);
        elapsedTimeSinceLastShot = 0;
    }
}

void Ship::handleMovement(float elapsedTime) {
    if (isThrothling) {
        float distance = (SPEED_IN_PIXELS_PER_SECOND) / (1.0F / elapsedTime);
        deltaPosition = jimp::GeometricsHelper::deltaByAngle(getRotationAngle(), distance);
    }
    
    if ((isOutsideScreenRight() && deltaPosition.x >= 0) || (isOutsideScreenLeft() && deltaPosition.x <= 0)) {
        deltaPosition.x = 0;
    }
    if ((isOutsideScreenAbove() && deltaPosition.y <= 0) || (isOutsideScreenBelow() && deltaPosition.y >= 0)) {
        deltaPosition.y = 0;
    }
    
    setX(getX() + deltaPosition.x);
    setY(getY() + deltaPosition.y);
}

void Ship::handleRotation(float elapsedTime) {
    if (isRotatingLeft || isRotatingRight) {
        float deltaDegrees = ROTATION_DEGREES_PER_SECOND / (1.0F / elapsedTime);
        if (isRotatingLeft) {
            deltaDegrees = -deltaDegrees;
        }
        setRotationAngle(getRotationAngle() + deltaDegrees);
    }
    
    updateAnimation(elapsedTime);
}
