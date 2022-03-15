#include "platformBlock.hpp"
#include "gameEngine.hpp"
#include "scrollingWorld.hpp"
#include "vector2D.hpp"

namespace jimp {
  
PlatformBlock::PlatformBlock(jimp::Vector2D position, std::string filePath, float angle, float scale, int zIndex) : jimp::AnimatedGraphic() {
    setPosition(position);
    setOffset(ScrollingWorld::getInstance()->getOffset());
    setApplyScrolling(true);
    setRotationAngle(angle);
    setCollidable(true);
    setScale(scale);
    setApplyGravity(false);
    setZIndex(zIndex);
    addSprite("default", filePath);
    name = filePath;
}

void PlatformBlock::doOnUpdate(float elapsedTime) {
    if (name == "platform-top-corner-right.png") {
        std::cout << "x: " << getPosition().x + getOffset().x << ", y: " << getPosition().y +  getOffset().y << std::endl;
    }
}

void PlatformBlock::doOnFrame(float elapsedTime) {
//    jimp::GameEngine::getInstance()->drawRectangle(getCollisionRectWidth(), getCollisionRectHeight(), jimp::Vector2D { .x = getPosition().x + getMarginLeft(), .y = getPosition().y + getMarginTop() }, 0xFFFFFF);
}
    
}
