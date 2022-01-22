#include <math.h>

#include "ship.hpp"
#include "fangoEventListener.hpp"
#include "bullet.hpp"
#include "direction.hpp"

const int Ship::SPEED_IN_PIXELS_PER_SECOND = 200;
const int Ship::ROTATION_DEGREES_PER_SECOND = 100;
const float Ship::IMAGE_SWAP_INTERVAL_IN_SECONDS = 0.1F;
const int Ship::SHOTS_PER_SECOND = 3;
const float Ship::SCALE = 0.2F;

Ship::Ship(jimp::GameEngine* gameEngine, FangoEventListener* eventListener) : jimp::AnimatedSprite(gameEngine, gameEngine->getScreenWidth() / 2, gameEngine->getScreenHeight() / 2, SCALE, IMAGE_SWAP_INTERVAL_IN_SECONDS) {
    getGameEngine().addKeyListener(this);
    this->eventListener = eventListener;
    addSprite("default", "spaceship.png");
}

void Ship::update(float elapsedTime) {
    handleFiring(elapsedTime);
    handleMovement(elapsedTime);
}

void Ship::onKeyboardLeft(jimp::KeyState keyState) {
    if (keyState == jimp::PRESSED) {
        directionX = Direction::LEFT;
    } else {
        directionX = Direction::IDLE;
    }
}

void Ship::onKeyboardRight(jimp::KeyState keyState) {
    if (keyState == jimp::PRESSED) {
        directionX = Direction::RIGHT;
    } else {
        directionX = Direction::IDLE;
    }
}

void Ship::onKeyboardUp(jimp::KeyState keyState) {
    if (keyState == jimp::PRESSED) {
        directionY = Direction::UP;
    } else {
        directionY = Direction::IDLE;
    }
}
void Ship::onKeyboardDown(jimp::KeyState keyState) {
    if (keyState == jimp::PRESSED) {
        directionY = Direction::DOWN;
    } else {
        directionY = Direction::IDLE;
    }
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
        Bullet* bullet = new Bullet(&getGameEngine(), getX() + (getWidth() / 2), getY() + (getHeight() / 2), Direction::RIGHT);
        eventListener->onWeaponFired(bullet);
        elapsedTimeSinceLastShot = 0;
    }
}

void Ship::handleMovement(float elapsedTime) {
    if (directionX == Direction::IDLE && directionY == Direction::IDLE) {
        return;
    }
    float delta = SPEED_IN_PIXELS_PER_SECOND / (1.0F / elapsedTime);
    float deltaDegrees = ROTATION_DEGREES_PER_SECOND / (1.0F / elapsedTime);
    float deltaX = 0;
    float deltaY = 0;
    if (directionX != Direction::IDLE && directionY != Direction::IDLE) {
        delta = sqrt((delta * delta) / 2);
    }
    std::string nextAnimation;
    switch (directionY) {
        case UP:
            deltaY = -delta;
            break;
        case DOWN:
            deltaY = delta;
            break;
    }
    switch (directionX) {
        case LEFT:
            setRotationAngle(-deltaDegrees);
            break;
        case RIGHT:
            setRotationAngle(deltaDegrees);
            break;
    }
    setX(getX() + deltaX);
    setY(getY() + deltaY);
    
    updateAnimation(elapsedTime);
}
