#include "platformBlock.hpp"
#include "gameEngine.hpp"
#include "scrollingWorld.hpp"
#include "vector2D.hpp"

namespace jimp {
  
PlatformBlock::PlatformBlock(jimp::Vector2D position, std::string filePath, float angle, float scale) : jimp::AnimatedGraphic(position, scale, angle, -1.0F, false) {
    name = "block";
    addSprite("default", filePath, 1);
}

void PlatformBlock::doOnInit() {
    setApplyScrolling(true);
}

void PlatformBlock::doOnUpdate(float elapsedTime) {
}

void PlatformBlock::doOnFrame(float elapsedTime) {
//    jimp::GameEngine::getInstance()->drawRectangle(getCollisionRectWidth(), getCollisionRectHeight(), jimp::Vector2D { .x = getPosition().x + getMarginLeft(), .y = getPosition().y + getMarginTop() }, 0xFFFFFF);
}

uint16_t PlatformBlock::getZIndex() {
    return 3;
}
    
}
