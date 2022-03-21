#include "ghosty.hpp"
#include "gameEngine.hpp"
#include "rectangle.hpp"
#include "vector2D.hpp"

namespace ghosty {

Ghosty::Ghosty() : jimp::AnimatedGraphic() {
    name = "ghosty";
    footstepSound = jimp::GameEngine::getInstance()->createNewSound("footstep.wav");
}

void Ghosty::doOnFrame(float elapsedTime) {
//    jimp::GameEngine::getInstance()->drawRectangle(getCollisionRectWidth(), getCollisionRectHeight(), jimp::Vector2D { .x = getPosition().x + getMarginLeft(), .y = getPosition().y + getMarginTop() }, 0xFFFFFF);
}

void Ghosty::doOnUpdate(float elapsedTime) {
    std::string currentAnimationId = getCurrentAnimationId();
    if (isJumping) {
        footstepSound->stop();
        setCurrentAnimation("jump");
    } else if (getVelocity().x != 0) {
        footstepSound->loop(60);
        setCurrentAnimation("walk");
    } else {
        footstepSound->stop();
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
        footstepSound->stop();
        stopMoving();
    }
}

void Ghosty::onKeyboardRight(jimp::KeyState keyState) {
    if (keyState == jimp::KeyState::PRESSED) {
        move(90, 500);
        drawInversedHorizontally(false);
    } else {
        footstepSound->stop();
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
