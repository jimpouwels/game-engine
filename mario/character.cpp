#include "character.hpp"
#include "gameEngine.hpp"
#include "rectangle.hpp"
#include "vector2D.hpp"

Character::Character() : jimp::AnimatedGraphic(jimp::Vector2D { .x = 10, .y = 10}, 0.5F, 0.0F, -1.0F) {
}

void Character::doOnInit() {
    addShape("default", new jimp::Rectangle(100, 100));
    jimp::GameEngine* gameEngine = jimp::GameEngine::getInstance();
    this->setPosition(jimp::Vector2D { .x = 10, .y = 400 });
}

void Character::doOnUpdate(float elapsedTime) {
   
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
