#include "character.hpp"
#include "gameEngine.hpp"

Character::Character() {
    position = jimp::Vector2D { .x = jimp::GameEngine::getInstance()->getScreenWidth() / 2.0F - 25.0F,
        .y = jimp::GameEngine::getInstance()->getScreenHeight() - 25.0F };
}

void Character::onUpdate(float elapsedTime) {
    
}

void Character::onFrame(float elapsedTime) {
    jimp::GameEngine::getInstance()->drawRectangle(50, 50, position);
}
