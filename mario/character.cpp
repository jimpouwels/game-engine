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
