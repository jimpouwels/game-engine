#include "platform.hpp"
#include "rectangle.hpp"
#include "gameEngine.hpp"

namespace mario {

Platform::Platform(jimp::Vector2D position, uint16_t repeat) : jimp::AnimatedGraphic(position, 0.5F, 0.0F, -1.0F, false) {
    this->repeat = repeat;
}

void Platform::doOnInit() {
    addSprite("default", "block.png", repeat);
}

void Platform::doOnUpdate(float elapsedTime) {
    
}

void Platform::hasCollidedRectTop(AnimatedGraphic* otherGraphic) {
//    hit = true;
}

}
