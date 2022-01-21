#include <math.h>

#include "fangoCharacter.hpp"
#include "fangoEventListener.hpp"
#include "bullet.hpp"

const int FangoCharacter::SPEED_IN_PIXELS_PER_SECOND = 200;
const float FangoCharacter::IMAGE_SWAP_INTERVAL_IN_SECONDS = 0.1F;
const int FangoCharacter::SHOTS_PER_SECOND = 5;
const float FangoCharacter::SCALE = 0.6F;

FangoCharacter::FangoCharacter(FangoEventListener* eventListener) : jimp::AnimatedSprite(0, 0, SCALE, IMAGE_SWAP_INTERVAL_IN_SECONDS) {
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
    setMoving(keyState);
    if (isMoving) {
        moveDirectionX = MoveDirection::MIN;
    } else {
        moveDirectionX = MoveDirection::IDLE;
    }
}

void FangoCharacter::onKeyboardRight(jimp::KeyState keyState) {
    setMoving(keyState);
    if (isMoving) {
        moveDirectionX = MoveDirection::PLUS;
    } else {
        moveDirectionX = MoveDirection::IDLE;
    }
}

void FangoCharacter::onKeyboardUp(jimp::KeyState keyState) {
    setMoving(keyState);
    if (isMoving) {
        moveDirectionY = MoveDirection::MIN;
    } else {
        moveDirectionY = MoveDirection::IDLE;
    }
}

void FangoCharacter::onKeyboardSpaceBar(jimp::KeyState keyState) {
    if (!isFiring) {
        firstShot = true;
    }
    isFiring = keyState == jimp::PRESSED;
}

void FangoCharacter::setMoving(jimp::KeyState keyState) {
    isMoving = keyState == jimp::PRESSED;
}

void FangoCharacter::onKeyboardDown(jimp::KeyState keyState) {
    setMoving(keyState);
    if (isMoving) {
        moveDirectionY = MoveDirection::PLUS;
    } else {
        moveDirectionY = MoveDirection::IDLE;
    }
}

void FangoCharacter::handleFiring(float elapsedTime) {
    if (!isFiring) {
        return;
    }
    elapsedTimeSinceLastShot += elapsedTime;
    float timeBetweenShots = 1.0F / SHOTS_PER_SECOND;
    if (elapsedTimeSinceLastShot >= timeBetweenShots || firstShot) {
        firstShot = false;
        Bullet* bullet = new Bullet(getX(), getY() + (getHeight() / 2));
        eventListener->onWeaponFired(bullet);
        elapsedTimeSinceLastShot = 0;
    }
}

void FangoCharacter::handleMovement(float elapsedTime) {
    if (!isMoving) {
        return;
    }
    float delta = SPEED_IN_PIXELS_PER_SECOND / (1.0F / elapsedTime);
    float deltaX = 0;
    float deltaY = 0;
    if (moveDirectionX != MoveDirection::IDLE && moveDirectionY != MoveDirection::IDLE) {
        delta = sqrt((delta * delta) / 2);
    }
    std::string nextAnimation;
    switch (moveDirectionY) {
        case MIN:
            nextAnimation = "up";
            deltaY = -delta;
            break;
        case PLUS:
            nextAnimation = "down";
            deltaY = delta;
            break;
    }
    switch (moveDirectionX) {
        case MIN:
            nextAnimation = "left";
            deltaX = -delta;
            break;
        case PLUS:
            nextAnimation = "right";
            deltaX = delta;
            break;
    }
    setX(getX() + deltaX);
    setY(getY() + deltaY);
    
    setCurrentAnimation(nextAnimation);
    updateAnimation(elapsedTime);
}
