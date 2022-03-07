#include "character.hpp"
#include "gameEngine.hpp"
#include "rectangle.hpp"
#include "vector2D.hpp"
#include "floor.hpp"

namespace mario {

Character::Character(jimp::Vector2D position) : jimp::AnimatedGraphic(position, 0.5F, 0.0F, 0.06F, true) {
}

void Character::doOnInit() {
    setApplyScrolling(true);
    for (int i = 0; i < 20; i++) {
        addSprite("idle", "Chara - BlueIdle" + std::to_string(i) + ".png", 1);
    }
    for (int i = 0; i < 20; i++) {
        addSprite("run", "Chara_BlueWalk" + std::to_string(i) + ".png", 1);
    }
    for (int i = 0; i < 8; i++) {
        addSprite("jump", "CharaWizardJump_" + std::to_string(i) + ".png", 1);
    }
}

void Character::doOnUpdate(float elapsedTime) {
    std::cout << "bottom: " << getHeight() << std::endl;
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
    stayToRightOf(otherGraphic);
}

void Character::hasCollidedRectLeft(AnimatedGraphic* otherGraphic) {
    stayToLeftOf(otherGraphic);
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
