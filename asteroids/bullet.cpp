#include "geo2D.hpp"
#include "bullet.hpp"
#include "animatedGraphic.hpp"
#include "vector2D.hpp"
#include "gameEngine.hpp"
#include "asteroid.hpp"

const uint16_t Bullet::FORCE = 1200;
const float Bullet::SCALE = 0.3F;

Bullet::Bullet(jimp::Vector2D position, float angle) : jimp::AnimatedGraphic(position, SCALE, 0, -1) {
    addSprite("default", "bullet.png");
    setRotationAngle(angle);
    setDeleteOnLeaveScreen(true);
    markAsInitialized();
}

void Bullet::doOnInit() {
    
}

void Bullet::doOnUpdate(float elapsedTime) {
    jimp::Vector2D deltaVector = jimp::Geo2D::vectorFrom(getRotationAngle(), FORCE, elapsedTime);
    addToPosition(deltaVector);
}

void Bullet::doOnFrame(float elapsedTime) {
}

void Bullet::hasCollidedRect(AnimatedGraphic *otherSprite, jimp::Geo2D::Side side) {
    Asteroid* asteroid = dynamic_cast<Asteroid*>(otherSprite);
    if (asteroid != nullptr) {
        markForDeletion();
    }
}

uint16_t Bullet::getZIndex() {
    return 999;
}
