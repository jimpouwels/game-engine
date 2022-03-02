#include "platform.hpp"
#include "rectangle.hpp"
#include "gameEngine.hpp"

namespace mario {

Platform::Platform() : jimp::AnimatedGraphic(jimp::Vector2D { .x = 900, .y = 250 }, 0.5F, 0.0F, -1.0F, false) {
}

void Platform::doOnInit() {
    addShape("default", new jimp::Rectangle(500, 50, 0x9CA9B8));
}

void Platform::doOnUpdate(float elapsedTime) {
   
}

}
