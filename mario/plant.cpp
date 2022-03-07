#include "plant.hpp"
#include "gameEngine.hpp"
#include "scrollingWorld.hpp"

namespace mario {
  
Plant::Plant(jimp::Vector2D position, uint16_t repeat) : jimp::AnimatedGraphic(position, 0.5F, 0.0F, 0.05F, false) {
    this->setCollidable(false);
    this->repeat = repeat;
    for (int i = 0; i < 90; i++) {
        addSprite("default", "Plant3_" + std::to_string(i) + ".png", 1);
    }
}

void Plant::doOnInit() {
    setApplyScrolling(true);
}

void Plant::doOnUpdate(float elapsedTime) {
}

uint16_t Plant::getZIndex() {
    return 50;
}
    
}
