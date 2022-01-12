#include "fangoCharacter.hpp"
#include "animatedCharacter.hpp"
#include "sprite.hpp"
#include <iostream>
#include <math.h>

namespace jimp {

FangoCharacter::FangoCharacter() : jimp::AnimatedCharacter(0, 0, 15, 30, IMAGE_SWAP_INTERVAL_IN_SECONDS) {
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

const int FangoCharacter::SPEED_IN_PIXELS_PER_SECOND = 200;
const float FangoCharacter::IMAGE_SWAP_INTERVAL_IN_SECONDS = 0.1F;

void FangoCharacter::update(float elapsedTime) {
    if (!isMoving) {
        return;
    }
    
    float delta = SPEED_IN_PIXELS_PER_SECOND / (1.0F / elapsedTime);
    float deltaX = 0;
    float deltaY = 0;
    if (moveDirectionX != MoveDirection::IDLE && moveDirectionY != MoveDirection::IDLE) {
        delta = sqrt((delta * delta) / 2);
    }
    std::string nextCollection;
    switch (moveDirectionY) {
        case MIN:
            nextCollection = "up";
            deltaY = -delta;
            break;
        case PLUS:
            nextCollection = "down";
            deltaY = delta;
            break;
    }
    switch (moveDirectionX) {
        case MIN:
            nextCollection = "left";
            deltaX = -delta;
            break;
        case PLUS:
            nextCollection = "right";
            deltaX = delta;
            break;
    }
    setX(getX() + deltaX);
    setY(getY() + deltaY);
    
    switchToNextSprite(nextCollection, elapsedTime);
}

void FangoCharacter::onKeyboardLeft(KeyState keyState) {
    setMoving(keyState);
    if (isMoving) {
        moveDirectionX = MoveDirection::MIN;
    } else {
        moveDirectionX = MoveDirection::IDLE;
    }
}

void FangoCharacter::onKeyboardRight(KeyState keyState) {
    setMoving(keyState);
    if (isMoving) {
        moveDirectionX = MoveDirection::PLUS;
    } else {
        moveDirectionX = MoveDirection::IDLE;
    }
}

void FangoCharacter::onKeyboardUp(KeyState keyState) {
    setMoving(keyState);
    if (isMoving) {
        moveDirectionY = MoveDirection::MIN;
    } else {
        moveDirectionY = MoveDirection::IDLE;
    }
}

void FangoCharacter::onKeyboardDown(KeyState keyState) {
    setMoving(keyState);
    if (isMoving) {
        moveDirectionY = MoveDirection::PLUS;
    } else {
        moveDirectionY = MoveDirection::IDLE;
    }
}

void FangoCharacter::setMoving(KeyState keyState) {
    if (keyState == PRESSED) {
        isMoving = true;
    } else {
        isMoving = false;
    }
}

}
