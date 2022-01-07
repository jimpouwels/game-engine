#include "fangoCharacter.hpp"
#include "animatedCharacter.hpp"
#include "sprite.hpp"
#include <iostream>

namespace jimp {

FangoCharacter::FangoCharacter() : jimp::AnimatedCharacter(new jimp::Sprite(0, 0, "fango.png")) {
    speedInPixelsPerSecond = 200;
}

void FangoCharacter::updatePosition(float elapsedTime) {
    if (!isMoving) {
        return;
    }

    int delta = speedInPixelsPerSecond / (1 / elapsedTime);
    switch (moveDirection) {
        case NORTH:
            sprite->setY(sprite->getY() - delta);
            break;
        case SOUTH:
            sprite->setY(sprite->getY() + delta);
            break;
        case WEST:
            sprite->setX(sprite->getX() - delta);
            break;
        case EAST:
            sprite->setX(sprite->getX() + delta);
            break;
    }
}

void FangoCharacter::onKeyboardLeft(KeyState keyState) {
    setMoving(true, keyState, WEST);
}

void FangoCharacter::onKeyboardRight(KeyState keyState) {
    setMoving(true, keyState, EAST);
}

void FangoCharacter::onKeyboardUp(KeyState keyState) {
    setMoving(true, keyState, NORTH);
}

void FangoCharacter::onKeyboardDown(KeyState keyState) {
    setMoving(true, keyState, SOUTH);
}

void FangoCharacter::setMoving(bool isMoving, KeyState keyState, MoveDirection moveDirection) {
    if (keyState == PRESSED) {
        this->isMoving = true;
        this->moveDirection = moveDirection;
    } else {
        this->isMoving = false;
    }
}

}
