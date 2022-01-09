#include "fangoCharacter.hpp"
#include "animatedCharacter.hpp"
#include "sprite.hpp"
#include <iostream>

namespace jimp {

FangoCharacter::FangoCharacter() : jimp::AnimatedCharacter(0, 0, IMAGE_SWAP_INTERVAL_IN_SECONDS) {
    setCurrentSprite(loadSprite("right", "fango-right1.png"));
    loadSprite("right", "fango-right2.png");
}

const int FangoCharacter::SPEED_IN_PIXELS_PER_SECOND = 200;
const float FangoCharacter::IMAGE_SWAP_INTERVAL_IN_SECONDS = 0.1F;

void FangoCharacter::updatePosition(float elapsedTime) {
    if (!isMoving) {
        return;
    }
    
    int delta = SPEED_IN_PIXELS_PER_SECOND / (1 / elapsedTime);
    switch (moveDirection) {
        case NORTH:
            this->getCurrentSprite()->setY(this->getCurrentSprite()->getY() - delta);
            break;
        case SOUTH:
            this->getCurrentSprite()->setY(this->getCurrentSprite()->getY() + delta);
            break;
        case WEST:
            this->getCurrentSprite()->setX(this->getCurrentSprite()->getX() - delta);
            break;
        case EAST:
            this->getCurrentSprite()->setX(this->getCurrentSprite()->getX() + delta);
            break;
    }
    
    setNextSpriteInCollection("right", elapsedTime);
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
