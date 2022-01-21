#include "bullet.hpp"
#include "animatedSprite.hpp"

const int Bullet::SPEED_IN_PIXELS_PER_SECOND = 1200;
const float Bullet::SCALE = 0.3F;

Bullet::Bullet(float x, float y) : jimp::AnimatedSprite(x, y, SCALE, 0.0F) {
    addSprite("default", "bullet.png");
}

void Bullet::update(float elapsedTime) {
    float delta = SPEED_IN_PIXELS_PER_SECOND / (1.0F / elapsedTime);
    setX(getX() + delta);
    this->updateAnimation(elapsedTime);
}
