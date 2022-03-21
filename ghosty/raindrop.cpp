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
    getPosition().y = jimp::MathUtils::randomNumberBetween(0, jimp::GameEngine::getInstance()->getScreenHeight());
    getPosition().x = jimp::MathUtils::randomNumberBetween(0, jimp::GameEngine::getInstance()->getScreenWidth());
    speedX = jimp::MathUtils::randomNumberBetween(-170, -240);
    speedY = jimp::MathUtils::randomNumberBetween(800, 900);
}

void Raindrop::doOnUpdate(float elapsedTime) {
    getPosition().y += jimp::Timing::toValueForElapsedTime(speedY, elapsedTime);
    getPosition().x += jimp::Timing::toValueForElapsedTime(speedX, elapsedTime);
    if (getScreenPosition().y < 0) {
        getPosition().y += jimp::GameEngine::getInstance()->getScreenHeight();
    } else if (getScreenPosition().y > jimp::GameEngine::getInstance()->getScreenHeight()) {
        getPosition().y -= jimp::GameEngine::getInstance()->getScreenHeight();
    } else if (getScreenPosition().x < 0) {
        getPosition().x += jimp::GameEngine::getInstance()->getScreenWidth();
    } else if (getScreenPosition().x > jimp::GameEngine::getInstance()->getScreenWidth()) {
        getPosition().x -= jimp::GameEngine::getInstance()->getScreenWidth();
    }
}

}
