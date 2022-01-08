#include "fangoCharacter.hpp"
#include "animatedCharacter.hpp"
#include "sprite.hpp"
#include <iostream>

namespace jimp {

FangoCharacter::FangoCharacter() : jimp::AnimatedCharacter(0, 0, "fango.png") {
    speedInPixelsPerSecond = 200;
}

void FangoCharacter::updatePosition(float elapsedTime) {
    if (!isMoving) {
        return;
    }

    int delta = speedInPixelsPerSecond / (1 / elapsedTime);
    switch (moveDirection) {
        case NORTH:
            setY(getY() - delta);
            break;
        case SOUTH:
            setY(getY() + delta);
            break;
        case WEST:
            setX(getX() - delta);
            break;
        case EAST:
            setX(getX() + delta);
            break;
    }
}

void FangoCharacter::onKeyboardLeft(KeyState keyState) {
    setMoving(keyState, WEST);
}

void FangoCharacter::onKeyboardRight(KeyState keyState) {
    setMoving(keyState, EAST);
}

void FangoCharacter::onKeyboardUp(KeyState keyState) {
    setMoving(keyState, NORTH);
}

void FangoCharacter::onKeyboardDown(KeyState keyState) {
    setMoving(keyState, SOUTH);
}

void FangoCharacter::setMoving(KeyState keyState, MoveDirection moveDirection) {
    if (keyState == PRESSED) {
        this->isMoving = true;
        this->moveDirection = moveDirection;
    } else {
        this->isMoving = false;
    }
}

}
