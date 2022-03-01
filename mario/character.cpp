#include "character.hpp"
#include "gameEngine.hpp"
#include "rectangle.hpp"
#include "vector2D.hpp"

Character::Character() : jimp::AnimatedGraphic(jimp::Vector2D { .x = 10, .y = 10}, 0.5F, 0.0F, -1.0F, true) {
}

void Character::doOnInit() {
    addShape("default", new jimp::Rectangle(100, 100));
    this->setPosition(jimp::Vector2D { .x = 10, .y = 400 });
}

void Character::doOnUpdate(float elapsedTime) {
    if (getPosition().y >= 400) { // reached floor, replace with collision detection with gravitational blocker
        disableGravitationalEffect();
    } else if (!hasCollidedWithBlocker) {
        enableGravitationalEffect();
    }
    hasCollidedWithBlocker = false;
}

void Character::hasCollidedRectBottom(AnimatedGraphic* otherGraphic) {
    disableGravitationalEffect();
    hasCollidedWithBlocker = true;
}


void Character::hasCollidedRectTop(AnimatedGraphic* otherGraphic) {
    getJumpVelocity().y = 0;
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
