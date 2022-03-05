#include "backgroundRedCharacter.hpp"
#include "animatedGraphic.hpp"
#include "mathUtils.hpp"

namespace mario {

BackgroundRedCharacter::BackgroundRedCharacter(jimp::Vector2D position) : jimp::AnimatedGraphic(position, 5.0F, 0.0F, 1.0F, false) {

}

void BackgroundRedCharacter::doOnInit() {
    addSprite("eyes-closed", "background-red-thingy-eyesclosed.png", 1);
    addSprite("eyes-right-eye-open", "background-red-thingy-eyeopenright.png", 1);
    addSprite("eyes-left-eye-open", "background-red-thingy-eyeopenleft.png", 1);
    setApplyScrolling(true);
    setCollidable(false);
}

void BackgroundRedCharacter::doOnUpdate(float elapsedTime) {
    std::string currentAnimationId = getCurrentAnimationId();
    if (currentAnimationId == "eyes-closed") {
        eyesClosedTime += elapsedTime;
        if (eyesClosedTime > 3) {
            jimp::MathUtils::randomNumberBetween(0, 2) == 0 ? setCurrentAnimation("eyes-right-eye-open") : setCurrentAnimation("eyes-left-eye-open");
            eyesClosedTime = 0;
        }
    } else if (currentAnimationId != "eyes-closed") {
        eyeOpenTime += elapsedTime;
        if (eyeOpenTime > 0.4F) {
            setCurrentAnimation("eyes-closed");
            eyeOpenTime = 0;
        }
    }
}

uint16_t BackgroundRedCharacter::getZIndex()  {
    return 999;
}

}
