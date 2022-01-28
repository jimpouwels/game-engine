#include <math.h>
#include "asteroid.hpp"
#include "screen.hpp"

#include <iostream>

const int Asteroid::SPEED_IN_PIXELS_PER_SECOND = 50;
const int Asteroid::ROTATION_DEGREES_PER_SECOND = 30;

Asteroid::Asteroid(jimp::Screen* screen, float x, float y) : jimp::AnimatedSprite(screen, x, y, 0.1F, -1) {
    addSprite("default", "asteroid.png");
    setRotationAngle(90);
}

Asteroid::~Asteroid() {
    
}

void Asteroid::update(float elapsedTime) {
    float deltaPosition = (SPEED_IN_PIXELS_PER_SECOND) / (1.0F / elapsedTime);
    setX(getX() + deltaPosition);
    setY(getY() + deltaPosition);
    

    float deltaDegrees = ROTATION_DEGREES_PER_SECOND / (1.0F / elapsedTime);
    setRotationAngle(getRotationAngle() + deltaDegrees);

    this->updateAnimation(elapsedTime);
}
