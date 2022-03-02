#include "platform.hpp"
#include "rectangle.hpp"
#include "gameEngine.hpp"

namespace mario {

Platform::Platform(jimp::Vector2D position, int width) : jimp::AnimatedGraphic(position, 0.5F, 0.0F, -1.0F, false) {
    this->width = width;
}

void Platform::doOnInit() {
    addShape("default", new jimp::Rectangle(width, 50, 0x9CA9B8));
}

void Platform::doOnUpdate(float elapsedTime) {
   
}

}
