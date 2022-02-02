#include "ship.hpp"
#include "shipEventListener.hpp"
#include "bullet.hpp"
#include "direction.hpp"
#include "geo2D.hpp"
#include "point2D.hpp"
#include "timing.hpp"

const int Ship::SPEED_IN_PIXELS_PER_SECOND = 200;
const int Ship::ROTATION_DEGREES_PER_SECOND = 140;
const int Ship::SHOTS_PER_SECOND = 10;
const float Ship::SCALE = 0.2F;

Ship::Ship(jimp::Screen* screen, ShipEventListener* eventListener) : jimp::AnimatedSprite(screen, screen->getWidth() / 2, screen->getHeight() / 2, SCALE, -1) {
    this->eventListener = eventListener;
    addSprite("default", "spaceship.png");
}

void Ship::update(float elapsedTime) {
    updateFiring(elapsedTime);
    updateMovement(elapsedTime);
    updateRotation(elapsedTime);
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
    hasFired = keyState == jimp::KeyState::PRESSED;
    isFiring = keyState == jimp::KeyState::PRESSED;
}

void Ship::updateFiring(float elapsedTime) {
    elapsedTimeSinceLastShot += elapsedTime;
    if (!isFiring && !hasFired) {
        return;
    }
    float timeBetweenShots = 1.0F / SHOTS_PER_SECOND;
    if (elapsedTimeSinceLastShot >= timeBetweenShots && (hasFired || isFiring)) {
        hasFired = false;
        Bullet* bullet = new Bullet(&getScreen(), getPosition().x + (getWidth() / 2), getPosition().y + (getHeight() / 2), getRotationAngle());
        eventListener->onWeaponFired(bullet);
        elapsedTimeSinceLastShot = 0;
    }
}

void Ship::updateMovement(float elapsedTime) {
    if (isThrothling) {
        float distance = jimp::Timing::toValueForElapsedTime(SPEED_IN_PIXELS_PER_SECOND, elapsedTime);
        deltaPosition = jimp::Geo2D::deltaByAngle(getRotationAngle(), distance);
    }
    
    if ((isOutsideScreenRight() && deltaPosition.x >= 0) || (isOutsideScreenLeft() && deltaPosition.x <= 0)) {
        deltaPosition.x = 0;
    }
    if ((isOutsideScreenAbove() && deltaPosition.y <= 0) || (isOutsideScreenBelow() && deltaPosition.y >= 0)) {
        deltaPosition.y = 0;
    }
    addToPosition(deltaPosition);
}

void Ship::updateRotation(float elapsedTime) {
    if (isRotatingLeft || isRotatingRight) {
        float deltaDegrees = jimp::Timing::toValueForElapsedTime(ROTATION_DEGREES_PER_SECOND, elapsedTime);
        if (isRotatingLeft) {
            deltaDegrees = -deltaDegrees;
        }
        setRotationAngle(jimp::Geo2D::normalizeAngle(getRotationAngle() + deltaDegrees));
    }
    
    updateAnimation(elapsedTime);
}
