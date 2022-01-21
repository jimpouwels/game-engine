#include <math.h>

#include "fangoCharacter.hpp"
#include "fangoEventListener.hpp"
#include "bullet.hpp"
#include "direction.hpp"

const int FangoCharacter::SPEED_IN_PIXELS_PER_SECOND = 200;
const float FangoCharacter::IMAGE_SWAP_INTERVAL_IN_SECONDS = 0.1F;
const int FangoCharacter::SHOTS_PER_SECOND = 3;
const float FangoCharacter::SCALE = 0.6F;

FangoCharacter::FangoCharacter(jimp::GameEngine* gameEngine, FangoEventListener* eventListener) : jimp::AnimatedSprite(gameEngine, 0, 0, SCALE, IMAGE_SWAP_INTERVAL_IN_SECONDS) {
    getGameEngine().addKeyListener(this);
    this->eventListener = eventListener;
    addSprite("right", "fango-right1.png");
    addSprite("right", "fango-right2.png");
    addSprite("right", "fango-right3.png");
    addSprite("right", "fango-right4.png");
    addSprite("left", "fango-left1.png");
    addSprite("left", "fango-left2.png");
    addSprite("left", "fango-left3.png");
    addSprite("left", "fango-left4.png");
    addSprite("down", "fango-down1.png");
    addSprite("down", "fango-down2.png");
    addSprite("down", "fango-down3.png");
    addSprite("down", "fango-down4.png");
    addSprite("up", "fango-up1.png");
    addSprite("up", "fango-up2.png");
    addSprite("up", "fango-up3.png");
    addSprite("up", "fango-up4.png");
}

void FangoCharacter::update(float elapsedTime) {
    handleFiring(elapsedTime);
    handleMovement(elapsedTime);
}

void FangoCharacter::onKeyboardLeft(jimp::KeyState keyState) {
    if (keyState == jimp::PRESSED) {
        directionX = Direction::LEFT;
    } else {
        directionX = Direction::IDLE;
    }
}

void FangoCharacter::onKeyboardRight(jimp::KeyState keyState) {
    if (keyState == jimp::PRESSED) {
        directionX = Direction::RIGHT;
    } else {
        directionX = Direction::IDLE;
    }
}

void FangoCharacter::onKeyboardUp(jimp::KeyState keyState) {
    if (keyState == jimp::PRESSED) {
        directionY = Direction::UP;
    } else {
        directionY = Direction::IDLE;
    }
}
void FangoCharacter::onKeyboardDown(jimp::KeyState keyState) {
    if (keyState == jimp::PRESSED) {
        directionY = Direction::DOWN;
    } else {
        directionY = Direction::IDLE;
    }
}

void FangoCharacter::onKeyboardSpaceBar(jimp::KeyState keyState) {
    if (keyState == jimp::KeyState::PRESSED) {
        hasFired = keyState == jimp::KeyState::PRESSED;
    }
    isFiring = keyState == jimp::KeyState::PRESSED;
}

void FangoCharacter::handleFiring(float elapsedTime) {
    elapsedTimeSinceLastShot += elapsedTime;
    if (!isFiring && !hasFired) {
        return;
    }
    float timeBetweenShots = 1.0F / SHOTS_PER_SECOND;
    if (elapsedTimeSinceLastShot >= timeBetweenShots && (hasFired || isFiring)) {
        hasFired = false;
        Bullet* bullet = new Bullet(&getGameEngine(), getX() + (getWidth() / 2), getY() + (getHeight() / 2), facingDirection);
        eventListener->onWeaponFired(bullet);
        elapsedTimeSinceLastShot = 0;
    }
}

void FangoCharacter::handleMovement(float elapsedTime) {
    if (directionX == Direction::IDLE && directionY == Direction::IDLE) {
        return;
    }
    float delta = SPEED_IN_PIXELS_PER_SECOND / (1.0F / elapsedTime);
    float deltaX = 0;
    float deltaY = 0;
    if (directionX != Direction::IDLE && directionY != Direction::IDLE) {
        delta = sqrt((delta * delta) / 2);
    }
    std::string nextAnimation;
    switch (directionY) {
        case UP:
            facingDirection = Direction::UP;
            nextAnimation = "up";
            deltaY = -delta;
            break;
        case DOWN:
            facingDirection = Direction::DOWN;
            nextAnimation = "down";
            deltaY = delta;
            break;
    }
    switch (directionX) {
        case LEFT:
            facingDirection = Direction::LEFT;
            nextAnimation = "left";
            deltaX = -delta;
            break;
        case RIGHT:
            facingDirection = Direction::RIGHT;
            nextAnimation = "right";
            deltaX = delta;
            break;
    }
    setX(getX() + deltaX);
    setY(getY() + deltaY);
    
    setCurrentAnimation(nextAnimation);
    updateAnimation(elapsedTime);
}
