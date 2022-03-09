#include "platformBlock.hpp"
#include "gameEngine.hpp"
#include "scrollingWorld.hpp"

namespace ghosty {
  
PlatformBlock::PlatformBlock(jimp::Vector2D position, std::string filePath, float angle, float scale) : jimp::AnimatedGraphic(position, scale, angle, -1.0F, false) {
    name = filePath;
    addSprite("default", filePath, 1);
}

void PlatformBlock::doOnInit() {
    setApplyScrolling(true);
}

void PlatformBlock::doOnUpdate(float elapsedTime) {
}

uint16_t PlatformBlock::getZIndex() {
    return 3;
}
    
}
