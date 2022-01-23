#include "bullet.hpp"
#include "animatedSprite.hpp"
#include "direction.hpp"

const int Bullet::SPEED_IN_PIXELS_PER_SECOND = 1200;
const float Bullet::SCALE = 0.3F;

Bullet::Bullet(jimp::GameEngine* gameEngine, float x, float y, Direction direction) : jimp::AnimatedSprite(gameEngine, x, y, SCALE, 0.0F) {
    this->direction = direction;
    addSprite("default", "bullet.png");
}

void Bullet::update(float elapsedTime) {
    float delta = SPEED_IN_PIXELS_PER_SECOND / (1.0F / elapsedTime);
    switch (direction) {
        case RIGHT:
            rotate(0);
            setX(getX() + delta);
            break;
        case LEFT:
            rotate(180);
            setX(getX() - delta);
            break;
        case UP:
            rotate(270);
            setY(getY() - delta);
            break;
        case DOWN:
            rotate(90);
            setY(getY() + delta);
            break;
    }
    this->updateAnimation(elapsedTime);
}
