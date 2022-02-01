#include "geo2D.hpp"
#include "bullet.hpp"
#include "animatedSprite.hpp"
#include "direction.hpp"

const int Bullet::SPEED_IN_PIXELS_PER_SECOND = 1200;
const float Bullet::SCALE = 0.3F;

Bullet::Bullet(jimp::Screen* screen, float x, float y, float angle) : jimp::AnimatedSprite(screen, x, y, SCALE, -1) {
    addSprite("default", "bullet.png");
    setRotationAngle(angle);
}

void Bullet::update(float elapsedTime) {
    float distance = SPEED_IN_PIXELS_PER_SECOND / (1.0F / elapsedTime);
    jimp::Point2D deltaPosition = jimp::Geo2D::deltaByAngle(getRotationAngle(), distance);
    addToPosition(deltaPosition);
    this->updateAnimation(elapsedTime);
}
