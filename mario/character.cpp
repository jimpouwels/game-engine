#include "character.hpp"
#include "gameEngine.hpp"
#include "rectangle.hpp"
#include "vector2D.hpp"

Character::Character() : jimp::AnimatedGraphic(jimp::Vector2D { .x = 10, .y = 10}, 0.5F, 0.0F, -1.0F) {
    jimp::GameEngine* gameEngine = jimp::GameEngine::getInstance();
    this->setPosition(jimp::Vector2D { .x = 10, .y = static_cast<float>((gameEngine->getScreenHeight() - getHeight())) });
}

void Character::doOnInit() {
    addShape("default", new jimp::Rectangle(100, 100));
}

void Character::doOnUpdate(float elapsedTime) {
    if (isMovingLeft) {
        move(270, 500, elapsedTime);
    } else if (isMovingRight) {
        move(90, 500, elapsedTime);
    }
}

void Character::onKeyboardLeft(jimp::KeyState keyState) {
    isMovingLeft = keyState == jimp::KeyState::PRESSED;
}

void Character::onKeyboardRight(jimp::KeyState keyState) {
    isMovingRight = keyState == jimp::KeyState::PRESSED;
}
