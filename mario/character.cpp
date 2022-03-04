#include "character.hpp"
#include "gameEngine.hpp"
#include "rectangle.hpp"
#include "vector2D.hpp"
#include "floor.hpp"

namespace mario {

Character::Character(jimp::Vector2D position) : jimp::AnimatedGraphic(position, 5.0F, 0.0F, 0.1F, true) {
}

void Character::doOnInit() {
    addSprite("idle-left", "mario-idleleft.png", 1);
    addSprite("idle-right", "mario-idleright.png", 1);
    addSprite("run-left", "mario-runleft1.png", 1);
    addSprite("run-left", "mario-runleft2.png", 1);
    addSprite("run-left", "mario-runleft3.png", 1);
    addSprite("run-right", "mario-runright1.png", 1);
    addSprite("run-right", "mario-runright2.png", 1);
    addSprite("run-right", "mario-runright3.png", 1);
    addSprite("jump-left", "mario-jumpleft.png", 1);
    addSprite("jump-right", "mario-jumpright.png", 1);
}

void Character::doOnUpdate(float elapsedTime) {
    std::string currentAnimationId = getCurrentAnimationId();
    if (isJumping) {
        if (getVelocity().x != 0) {
            getVelocity().x > 0 ? setCurrentAnimation("jump-right") : setCurrentAnimation("jump-left");
        }
    } else if (getVelocity().x > 0) {
        setCurrentAnimation("run-right");
    } else if (getVelocity().x < 0) {
        setCurrentAnimation("run-left");
    } else if (currentAnimationId == "jump-right" || currentAnimationId == "run-right") {
        setCurrentAnimation("idle-right");
    } else if (currentAnimationId == "jump-left" || currentAnimationId == "run-left") {
        setCurrentAnimation("idle-left");
    }
}

void Character::hasCollidedRectBottom(AnimatedGraphic* otherGraphic) {
    isJumping = false;
    stayOnTopOf(otherGraphic);
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
        if (!isJumping) {
            isJumping = true;
            jump(1500);
        }
    }
}

}
