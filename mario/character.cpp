#include "character.hpp"
#include "gameEngine.hpp"
#include "rectangle.hpp"
#include "vector2D.hpp"
#include "floor.hpp"

namespace mario {

Character::Character(jimp::Vector2D position) : jimp::AnimatedGraphic(position, 5.0F, 0.0F, 0.06F, true) {
}

void Character::doOnInit() {
    setApplyScrolling(true);
    addSprite("idle", "mario-idle.png", 1);
    addSprite("run", "mario-run1.png", 1);
    addSprite("run", "mario-run2.png", 1);
    addSprite("run", "mario-run3.png", 1);
    addSprite("jump", "mario-jump.png", 1);
}

void Character::doOnUpdate(float elapsedTime) {
    std::string currentAnimationId = getCurrentAnimationId();
    if (isJumping) {
        setCurrentAnimation("jump");
    } else if (getVelocity().x != 0) {
        setCurrentAnimation("run");
    } else {
        setCurrentAnimation("idle");
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
    if (dynamic_cast<Floor*>(otherGraphic) == nullptr) {
        stayToRightOf(otherGraphic);
    }
}

void Character::hasCollidedRectLeft(AnimatedGraphic* otherGraphic) {
    if (dynamic_cast<Floor*>(otherGraphic) == nullptr) {
        stayToLeftOf(otherGraphic);
    }
}

void Character::onKeyboardLeft(jimp::KeyState keyState) {
    if (keyState == jimp::KeyState::PRESSED) {
        move(270, 500);
        drawInversedHorizontally(true);
    } else {
        stopMoving();
    }
}

void Character::onKeyboardRight(jimp::KeyState keyState) {
    if (keyState == jimp::KeyState::PRESSED) {
        move(90, 500);
        drawInversedHorizontally(false);
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
