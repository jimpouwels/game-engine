#include "decoration.hpp"
#include "gameEngine.hpp"
#include "scrollingWorld.hpp"
#include <regex>

namespace mario {
  
Decoration::Decoration(jimp::Vector2D position, std::string filePath, int imageCount, float rotationAngle) : jimp::AnimatedGraphic(position, 0.5F, rotationAngle, 0.05F, false) {
    this->setCollidable(false);
    this->repeat = 1;
    for (int i = 0; i < imageCount; i++) {
        addSprite("default", std::regex_replace(filePath, std::regex("\\{i\\}"), std::to_string(i)), 1);
    }
}

void Decoration::doOnInit() {
    setApplyScrolling(true);
}

void Decoration::doOnUpdate(float elapsedTime) {
}

uint16_t Decoration::getZIndex() {
    return 50;
}
    
}
