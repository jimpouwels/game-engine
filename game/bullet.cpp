#include "geo2D.hpp"
#include "bullet.hpp"
#include "animatedSprite.hpp"
#include "direction.hpp"
#include "timing.hpp"

const int Bullet::SPEED_IN_PIXELS_PER_SECOND = 1200;
const float Bullet::SCALE = 0.3F;

Bullet::Bullet(jimp::GamingInterface* gamingInterface, float x, float y, float angle) : jimp::AnimatedSprite(gamingInterface, x, y, SCALE, -1) {
    addSprite("default", "bullet.png");
    setRotationAngle(angle);
}

void Bullet::onFrame(float elapsedTime) {
    float distance = jimp::Timing::toValueForElapsedTime(SPEED_IN_PIXELS_PER_SECOND, elapsedTime);
    jimp::Vector2D deltaVector = jimp::Geo2D::deltaVectorByAngle(getRotationAngle(), distance);
    addToPosition(deltaVector);
    
    draw(elapsedTime);
}
