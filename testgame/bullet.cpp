#include "geo2D.hpp"
#include "bullet.hpp"
#include "animatedSprite.hpp"
#include "direction.hpp"
#include "vector2D.hpp"

const uint16_t Bullet::FORCE = 1200;
const float Bullet::SCALE = 0.3F;

Bullet::Bullet(jimp::GameEngine* gameEngine, float x, float y, float angle) : jimp::AnimatedSprite(gameEngine, x, y, SCALE, -1) {
    addSprite("default", "bullet.png");
    setRotationAngle(angle);
}

void Bullet::onUpdate(float elapsedTime) {
    jimp::Vector2D deltaVector = jimp::Geo2D::vectorFrom(getRotationAngle(), FORCE, elapsedTime);
    addToPosition(deltaVector);
    
    AnimatedSprite::onUpdate(elapsedTime);
}

void Bullet::hasCollidedRect(AnimatedSprite *otherSprite, jimp::Geo2D::Side side) {
    markForDeletion();
}

bool Bullet::isPositionedWithinScreen() {
    jimp::Vector2D p = getPosition();
    return p.x <= getGameEngine()->getScreenWidth() + 400 && p.x >= -400 && p.y <= getGameEngine()->getScreenHeight() + 400 && p.y > -400;
}
