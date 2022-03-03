#include "floor.hpp"
#include "gameEngine.hpp"

namespace mario {
  
Floor::Floor(jimp::Vector2D position, uint16_t repeat) : jimp::AnimatedGraphic(position, 0.15F, 0.0F, -1.0F, false) {
    this->repeat = repeat;
}

void Floor::doOnInit() {
    addSprite("default", "floor.png", repeat);
    getPosition().y = jimp::GameEngine::getInstance()->getScreenHeight() - getHeight();
}

void Floor::doOnUpdate(float elapsedTime) {
   
}
    
}
