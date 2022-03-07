#include "floor.hpp"
#include "gameEngine.hpp"
#include "scrollingWorld.hpp"

namespace mario {
  
Floor::Floor(jimp::Vector2D position, std::string filePath, uint16_t repeat) : jimp::AnimatedGraphic(position, 0.5F, 0.0F, -1.0F, false) {
    this->repeat = repeat;
    name = filePath;
    addSprite("default", filePath, repeat);
    getPosition().y = jimp::ScrollingWorld::getInstance()->getHeight() - getHeight();
}

void Floor::doOnInit() {
    setApplyScrolling(true);
}

void Floor::doOnUpdate(float elapsedTime) {
   
}
    
}
