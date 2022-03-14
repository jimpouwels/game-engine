#include "simpleAnimation.hpp"
#include "gameEngine.hpp"
#include "scrollingWorld.hpp"
#include <regex>

namespace jimp {
  
SimpleAnimation::SimpleAnimation(jimp::Vector2D position, std::string filePath, int imageCount, float rotationAngle, float scale, int zIndex) : jimp::AnimatedGraphic(position, scale, rotationAngle, 0.05F, false) {
    this->zIndex = zIndex;
    this->name = filePath;
    this->setCollidable(false);
    this->repeat = 1;
    for (int i = 0; i < imageCount; i++) {
        addSprite("default", std::regex_replace(filePath, std::regex("\\{i\\}"), std::to_string(i)), 1);
    }
}

void SimpleAnimation::doOnInit() {
    setApplyScrolling(true);
}

void SimpleAnimation::doOnUpdate(float elapsedTime) {
}

uint16_t SimpleAnimation::getZIndex() {
    return zIndex;
}
    
}
