#include "bullet.hpp"
#include "animatedSprite.hpp"

Bullet::Bullet(float x, float y) : jimp::AnimatedSprite(x, y, 0.3, 0.0F) {
    addSprite("default", "bullet.png");
}

void Bullet::update(float elapsedTime) {
    float delta = 1200 / (1.0F / elapsedTime);
    setX(getX() + delta);
    this->updateAnimation(elapsedTime);
}
