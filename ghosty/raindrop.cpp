#include "raindrop.hpp"
#include "rectangle.hpp"
#include "color.hpp"
#include "gameEngine.hpp"
#include "mathUtils.hpp"

namespace ghosty {

Raindrop::Raindrop() {
    setName("raindrop");
    setApplyScrolling(true);
    setCollidable(false);
    addSprite("default", "raindrop.png");
    setScale(0.1f);
    setRotationAngle(20.0f);
    initialX = jimp::MathUtils::randomNumberBetween(0, jimp::ScrollingWorld::getInstance()->getWidth());
    getPosition().x = initialX;
    getPosition().y = jimp::MathUtils::randomNumberBetween(0, jimp::ScrollingWorld::getInstance()->getHeight());;
    speedX = jimp::MathUtils::randomNumberBetween(-170, -240);
    speedY = jimp::MathUtils::randomNumberBetween(800, 900);
}

void Raindrop::doOnUpdate(float elapsedTime) {
    getPosition().y += jimp::Timing::toValueForElapsedTime(speedY, elapsedTime);
    getPosition().x += jimp::Timing::toValueForElapsedTime(speedX, elapsedTime);
    if (getPosition().y > jimp::ScrollingWorld::getInstance()->getHeight()) {
        getPosition().y = 0;
        getPosition().x = initialX;
    }
}

}
