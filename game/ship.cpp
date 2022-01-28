#include <math.h>

#include "ship.hpp"
#include "shipEventListener.hpp"
#include "bullet.hpp"
#include "direction.hpp"

const int Ship::SPEED_IN_PIXELS_PER_SECOND = 200;
const int Ship::ROTATION_DEGREES_PER_SECOND = 140;
const int Ship::SHOTS_PER_SECOND = 3;
const float Ship::SCALE = 0.2F;

Ship::Ship(ShipEventListener* eventListener) : jimp::AnimatedSprite(100, 100, SCALE, -1) {
    this->eventListener = eventListener;
    addSprite("default", "spaceship.png");
}

void Ship::update(float elapsedTime) {
    handleFiring(elapsedTime);
    handleMovement(elapsedTime);
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
        Bullet* bullet = new Bullet(getX() + (getWidth() / 2), getY() + (getHeight() / 2), getRotationAngle());
        eventListener->onWeaponFired(bullet);
        elapsedTimeSinceLastShot = 0;
    }
}

void Ship::handleMovement(float elapsedTime) {
    if (isThrothling) {
        float delta = (SPEED_IN_PIXELS_PER_SECOND) / (1.0F / elapsedTime);
        velocityX = delta * sin(M_PI * 2 * getRotationAngle() / 360);
        velocityY = delta * cos(M_PI * 2 * getRotationAngle() / 360);
    }
    setX(getX() + velocityX);
    setY(getY() + -velocityY);
    
    if (isRotatingLeft || isRotatingRight) {
        float deltaDegrees = ROTATION_DEGREES_PER_SECOND / (1.0F / elapsedTime);
        if (isRotatingLeft) {
            deltaDegrees = -deltaDegrees;
        }
        rotate(deltaDegrees);
    }
    
    updateAnimation(elapsedTime);
}
