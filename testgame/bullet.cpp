#include "geo2D.hpp"
#include "bullet.hpp"
#include "animatedSprite.hpp"
#include "direction.hpp"
#include "timing.hpp"

const int Bullet::FORCE = 1200;
const float Bullet::SCALE = 0.3F;

Bullet::Bullet(jimp::GameEngine* gameEngine, float x, float y, float angle) : jimp::AnimatedSprite(gameEngine, x, y, SCALE, -1) {
    addSprite("default", "bullet.png");
    setRotationAngle(angle);
}

void Bullet::onFrame(float elapsedTime) {
    float force = jimp::Timing::toValueForElapsedTime(FORCE, elapsedTime);
    jimp::Vector2D deltaVector = jimp::Geo2D::vectorFrom(getRotationAngle(), force);
    addToPosition(deltaVector);
    
    draw(elapsedTime);
}
