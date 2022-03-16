#include "geo2D.hpp"
#include "bullet.hpp"
#include "animatedGraphic.hpp"
#include "vector2D.hpp"
#include "gameEngine.hpp"
#include "asteroid.hpp"

const uint16_t Bullet::FORCE = 1200;
const float Bullet::SCALE = 0.3F;

Bullet::Bullet(jimp::Vector2D position, float angle) : jimp::AnimatedGraphic() {
    setRotationAngle(angle);
    setPosition(position);
    setScale(0.2);
    setCollidable(true);
    setZIndex(1);
    addSprite("default", "bullet.png");
    setDeleteOnLeaveScreen(true);
    name = "bullet";
}

void Bullet::doOnUpdate(float elapsedTime) {
    move(getRotationAngle(), FORCE);
}

void Bullet::doOnFrame(float elapsedTime) {
}

void Bullet::hasCollidedRect(AnimatedGraphic *otherSprite, jimp::Geo2D::Side side) {
    Asteroid* asteroid = dynamic_cast<Asteroid*>(otherSprite);
    if (asteroid != nullptr) {
        markForDeletion();
    }
}
