#include "character.hpp"
#include "gameEngine.hpp"
#include "rectangle.hpp"
#include "vector2D.hpp"

Character::Character() : jimp::Graphic(jimp::Vector2D { .x = 10, .y = 10}, 0.5F, 0.0F, -1.0F) {
    addShape("default", new jimp::Rectangle(100, 100));
    jimp::GameEngine* gameEngine = jimp::GameEngine::getInstance();
    this->setPosition(jimp::Vector2D { .x = 10, .y = static_cast<float>((gameEngine->getScreenHeight() - getHeight())) });
    markAsInitialized();
}

void Character::doOnUpdate(float elapsedTime) {
    if (!isMovingLeft && !isMovingRight) {
        return;
    }
    if (!(isMovingLeft && getPosition().x <= 0) &&
        !(isMovingRight && getPosition().x >= (jimp::GameEngine::getInstance()->getScreenWidth() - getWidth()))) {
        jimp::Vector2D delta = jimp::Vector2D { .x = 500, .y = 0 };
        if (isMovingLeft) {
            delta.x = -delta.x;
        }
        addToPosition(jimp::Timing::toValueForElapsedTime(delta, elapsedTime));
    }
}

void Character::onKeyboardLeft(jimp::KeyState keyState) {
    isMovingLeft = keyState == jimp::KeyState::PRESSED;
}

void Character::onKeyboardRight(jimp::KeyState keyState) {
    isMovingRight = keyState == jimp::KeyState::PRESSED;
}
