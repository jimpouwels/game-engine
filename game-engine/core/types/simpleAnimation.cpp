#include "simpleAnimation.hpp"
#include "gameEngine.hpp"
#include "scrollingWorld.hpp"
#include <regex>

namespace jimp {
  
SimpleAnimation::SimpleAnimation() : jimp::AnimatedGraphic() {
//    this->zIndex = zIndex;
//    this->name = filePath;
//    this->setCollidable(false);
//    this->repeat = 1;
//    for (int i = 0; i < imageCount; i++) {
//        addSprite("default", std::regex_replace(filePath, std::regex("\\{i\\}"), std::to_string(i)), 1);
//    }
}

void SimpleAnimation::doOnUpdate(float elapsedTime) {
}

uint16_t SimpleAnimation::getZIndex() {
    return zIndex;
}
    
}
