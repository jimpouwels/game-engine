#include "platform.hpp"
#include "rectangle.hpp"
#include "gameEngine.hpp"

namespace mario {

Platform::Platform(jimp::Vector2D position) : jimp::AnimatedGraphic(position, 0.5F, 0.0F, -1.0F, false) {
}

void Platform::doOnInit() {
    addSprite("default", "block.png");
}

void Platform::doOnUpdate(float elapsedTime) {
   
}

}
