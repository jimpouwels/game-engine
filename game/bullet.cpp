#include "bullet.hpp"
#include "animatedSprite.hpp"

Bullet::Bullet(float x, float y, float scale) : jimp::AnimatedSprite(x, y, scale, 0.0F) {
    addSprite("default", "bullet.png");
}

void Bullet::update(float elapsedTime) {
    // update position
    this->updateAnimation(elapsedTime);
}
