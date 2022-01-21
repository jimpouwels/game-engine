#include "bullet.hpp"
#include "animatedSprite.hpp"

Bullet::Bullet(float x, float y, float scale) : jimp::AnimatedSprite(x, y, scale, 0.0F) {
    addSprite("default", "bullet.png");
}

void Bullet::update(float elapsedTime) {
    float delta = 400 / (1.0F / elapsedTime);
    setX(getX() + delta);
    this->updateAnimation(elapsedTime);
}
