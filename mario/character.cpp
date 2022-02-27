#include "character.hpp"
#include "gameEngine.hpp"
#include "rectangle.hpp"
#include "vector2D.hpp"

Character::Character() : jimp::Graphic(jimp::Vector2D { .x = 10, .y = 10}, 1.0F, 0.0F, -1.0F) {
    addShape("default", new jimp::Rectangle(100, 100));
}
