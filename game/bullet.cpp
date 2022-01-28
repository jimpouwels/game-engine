#include <math.h>
#include "bullet.hpp"
#include "animatedSprite.hpp"
#include "direction.hpp"

const int Bullet::SPEED_IN_PIXELS_PER_SECOND = 1200;
const float Bullet::SCALE = 0.3F;

Bullet::Bullet(float x, float y, float angle) : jimp::AnimatedSprite(x, y, SCALE, -1) {
    addSprite("default", "bullet.png");
    setRotationAngle(angle);
}

void Bullet::update(float elapsedTime) {
    float delta = SPEED_IN_PIXELS_PER_SECOND / (1.0F / elapsedTime);
    float deltaX = delta * sin(M_PI * 2 * (getRotationAngle()) / 360);
    float deltaY = delta * cos(M_PI * 2 * (getRotationAngle()) / 360);
    setX(getX() + deltaX);
    setY(getY() + -deltaY);
    this->updateAnimation(elapsedTime);
}
