#include "movingPlatform.hpp"

namespace ghosty {

MovingPlatform::MovingPlatform() {
    move(180, 300);
}

void MovingPlatform::doOnUpdate(float elapsedTime) {
    if (initialY < 0) {
        initialY = getPosition().y;
    }
    if (getPosition().y > initialY + 500) {
        move(0, 300);
    } else if (getPosition().y < initialY - 500) {
        move(180, 300);
    }
}

}
