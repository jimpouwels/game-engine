#include "ghosty.hpp"
#include "gameEngine.hpp"
#include "rectangle.hpp"
#include "vector2D.hpp"

namespace ghosty {

Ghosty::Ghosty() : jimp::AnimatedGraphic() {
    name = "ghosty";
    footstepSound = jimp::GameEngine::getInstance()->createNewSound("footstep.wav");
    jumpSound = jimp::GameEngine::getInstance()->createNewSound("jump.wav");
}

void Ghosty::doOnFrame(float elapsedTime) {
    jimp::Vector2D posTextPosition = jimp::Vector2D::from(getPosition().x, getPosition().y);
    posTextPosition.y -= 10;
    std::string text = "Pos: " + std::to_string(getPosition().x) + ", " + std::to_string(getPosition().y);
    text += "\n";
    text += "Velocity: " + std::to_string(getVelocity().x) + ", " + std::to_string(getVelocity().y);
    jimp::GameEngine::getInstance()->draw(text , posTextPosition);
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

void Ghosty::hasNoCollisionWith(AnimatedGraphic *otherGraphic) {
    stopStayOnTopOf();
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
            jumpSound->play(20);
            isJumping = true;
            jump(2100);
        }
    }
}

}
