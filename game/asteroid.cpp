#include <math.h>
#include "asteroid.hpp"
#include "screen.hpp"

#include <iostream>

const int Asteroid::SPEED_IN_PIXELS_PER_SECOND = 150;
const int Asteroid::ROTATION_DEGREES_PER_SECOND = 30;

Asteroid::Asteroid(jimp::Screen* screen, float x, float y) : jimp::AnimatedSprite(screen, x, y, 0.1F, -1) {
    addSprite("default", "asteroid.png");
    setRotationAngle(90);
    directionX = DirectionX::RIGHT;
    directionY = DirectionY::DOWN;
}

Asteroid::~Asteroid() {
    
}

void Asteroid::update(float elapsedTime) {
    float deltaPosition = (SPEED_IN_PIXELS_PER_SECOND) / (1.0F / elapsedTime);
    
    if (getY() >= getScreen().getHeight() - getHeight()) {
        directionY = DirectionY::UP;
    } else if (getY() <= 0) {
        directionY = DirectionY::DOWN;
    }
    if (getX() >= getScreen().getWidth() - getWidth()) {
        directionX = DirectionX::LEFT;
    } else if (getX() <= 0) {
        directionX = DirectionX::RIGHT;
    }
    
    switch (directionY) {
        case UP:
            setY(getY() - deltaPosition);
            break;
        case DOWN:
            setY(getY() + deltaPosition);
            break;
    }
    switch (directionX) {
        case RIGHT:
            setX(getX() + deltaPosition);
            break;
        case LEFT:
            setX(getX() - deltaPosition);
            break;
    }

    float deltaDegrees = ROTATION_DEGREES_PER_SECOND / (1.0F / elapsedTime);
    setRotationAngle(getRotationAngle() + deltaDegrees);

    this->updateAnimation(elapsedTime);
}
