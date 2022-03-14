#include "ghosty.hpp"
#include "gameEngine.hpp"
#include "rectangle.hpp"
#include "vector2D.hpp"
#include "platformBlock.hpp"

namespace ghosty {

Ghosty::Ghosty(jimp::Vector2D position) : jimp::AnimatedGraphic(position, 0.5F, 0.0F, 0.06F, true) {
    name = "ghosty";
}

void Ghosty::doOnInit() {
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

void Ghosty::doOnFrame(float elapsedTime) {
//    jimp::GameEngine::getInstance()->drawRectangle(getCollisionRectWidth(), getCollisionRectHeight(), jimp::Vector2D { .x = getPosition().x + getMarginLeft(), .y = getPosition().y + getMarginTop() }, 0xFFFFFF);
}

void Ghosty::doOnUpdate(float elapsedTime) {
    std::string currentAnimationId = getCurrentAnimationId();
    if (isJumping) {
        setCurrentAnimation("jump");
    } else if (getVelocity().x != 0) {
        setCurrentAnimation("run");
    } else {
        setCurrentAnimation("idle");
    }
}

void Ghosty::hasCollidedRectBottom(AnimatedGraphic* otherGraphic) {
    isJumping = false;
    stayOnTopOf(otherGraphic);
}

void Ghosty::hasCollidedRectTop(AnimatedGraphic* otherGraphic) {
    interruptJump();
}

void Ghosty::hasCollidedRectRight(AnimatedGraphic* otherGraphic) {
    stayToLeftOf(otherGraphic);
}

void Ghosty::hasCollidedRectLeft(AnimatedGraphic* otherGraphic) {
    stayToRightOf(otherGraphic);
}

void Ghosty::onKeyboardLeft(jimp::KeyState keyState) {
    if (keyState == jimp::KeyState::PRESSED) {
        move(270, 500);
        drawInversedHorizontally(true);
    } else {
        stopMoving();
    }
}

void Ghosty::onKeyboardRight(jimp::KeyState keyState) {
    if (keyState == jimp::KeyState::PRESSED) {
        move(90, 500);
        drawInversedHorizontally(false);
    } else {
        stopMoving();
    }
}

void Ghosty::onKeyboardUp(jimp::KeyState keyState) {
    if (keyState == jimp::KeyState::PRESSED) {
        if (!isJumping) {
            isJumping = true;
            jump(2100);
        }
    }
}

}
