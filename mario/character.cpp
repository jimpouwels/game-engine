#include "character.hpp"
#include "gameEngine.hpp"
#include "rectangle.hpp"
#include "vector2D.hpp"

namespace mario {

Character::Character(jimp::Vector2D position) : jimp::AnimatedGraphic(position, 0.5F, 0.0F, -1.0F, true) {
}

void Character::doOnInit() {
    addShape("default", new jimp::Rectangle(100, 100, 0xD19F9C));
}

void Character::doOnUpdate(float elapsedTime) {
    if (getPosition().y >= 400) { // reached floor, replace with collision detection with gravitational blocker
        blockGravity();
    } else if (!hasCollidedWithBlocker) {
        unblockGravity();
    }
    hasCollidedWithBlocker = false;
}

void Character::hasCollidedRectBottom(AnimatedGraphic* otherGraphic) {
    blockGravity();
    hasCollidedWithBlocker = true;
}

void Character::hasCollidedRectTop(AnimatedGraphic* otherGraphic) {
    interruptJump();
}

void Character::hasCollidedRectRight(AnimatedGraphic* otherGraphic) {
    stopMoving();
    getMoveVelocity().x = - getMoveVelocity().x;
}

void Character::hasCollidedRectLeft(AnimatedGraphic* otherGraphic) {
    stopMoving();
    getMoveVelocity().x = - getMoveVelocity().x;
}

void Character::onKeyboardLeft(jimp::KeyState keyState) {
    if (keyState == jimp::KeyState::PRESSED) {
        move(270, 500);
    } else {
        stopMoving();
    }
}

void Character::onKeyboardRight(jimp::KeyState keyState) {
    if (keyState == jimp::KeyState::PRESSED) {
        move(90, 500);
    } else {
        stopMoving();
    }
}

void Character::onKeyboardUp(jimp::KeyState keyState) {
    if (keyState == jimp::KeyState::PRESSED) {
        if (!isJumping()) {
            jump(1500);
        }
    }
}

}
