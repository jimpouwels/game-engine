#include <math.h>
#include "bullet.hpp"
#include "animatedSprite.hpp"
#include "direction.hpp"

const int Bullet::SPEED_IN_PIXELS_PER_SECOND = 1200;
const float Bullet::SCALE = 0.3F;

Bullet::Bullet(jimp::GameEngine* gameEngine, float x, float y, float angle) : jimp::AnimatedSprite(gameEngine, x, y, SCALE, 0.0F) {
    addSprite("default", "bullet.png");
    setRotationAngle(angle - 90);
}

void Bullet::update(float elapsedTime) {
    float delta = SPEED_IN_PIXELS_PER_SECOND / (1.0F / elapsedTime);
    float velocityX = delta * sin(M_PI * 2 * (getRotationAngle() + 90) / 360);
    float velocityY = delta * cos(M_PI * 2 * (getRotationAngle() + 90) / 360);
    setX(getX() + velocityX);
    setY(getY() + -velocityY);
    this->updateAnimation(elapsedTime);
}
