#include "bullet.hpp"
#include "animatedSprite.hpp"
#include "direction.hpp"

const int Bullet::SPEED_IN_PIXELS_PER_SECOND = 1200;
const float Bullet::SCALE = 0.3F;

Bullet::Bullet(float x, float y, Direction direction) : jimp::AnimatedSprite(x, y, SCALE, 0.0F) {
    this->direction = direction;
    addSprite("default", "bullet.png");
}

void Bullet::update(float elapsedTime) {
    float delta = SPEED_IN_PIXELS_PER_SECOND / (1.0F / elapsedTime);
    switch (direction) {
        case EAST:
            setX(getX() + delta);
            break;
        case WEST:
            setX(getX() - delta);
            break;
        case NORTH:
            setY(getY() - delta);
            break;
        case SOUTH:
            setY(getY() + delta);
            break;
    }
    this->updateAnimation(elapsedTime);
}
