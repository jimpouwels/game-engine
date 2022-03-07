#include "floor.hpp"
#include "gameEngine.hpp"
#include "scrollingWorld.hpp"

namespace mario {
  
Floor::Floor(jimp::Vector2D position, uint16_t repeat) : jimp::AnimatedGraphic(position, 1.0F, 0.0F, -1.0F, false) {
    this->repeat = repeat;
}

void Floor::doOnInit() {
    setApplyScrolling(true);
    addSprite("default", "block.png", repeat);
    getPosition().y = jimp::ScrollingWorld::getInstance()->getHeight() - getHeight();
}

void Floor::doOnUpdate(float elapsedTime) {
   
}
    
}
