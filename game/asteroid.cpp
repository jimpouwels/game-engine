#include <math.h>
#include "asteroid.hpp"

#include <iostream>

const int Asteroid::SPEED_IN_PIXELS_PER_SECOND = 50;
const int Asteroid::ROTATION_DEGREES_PER_SECOND = 30;

Asteroid::Asteroid(float x, float y) : jimp::AnimatedSprite(x, y, 0.1F, -1) {
    addSprite("default", "asteroid.png");
    setRotationAngle(90);
}

Asteroid::~Asteroid() {
    
}

void Asteroid::update(float elapsedTime) {
    float deltaPosition = (SPEED_IN_PIXELS_PER_SECOND) / (1.0F / elapsedTime);
    std::cout << elapsedTime << ", " << (SPEED_IN_PIXELS_PER_SECOND) / (1.0F / elapsedTime) << std::endl;
    setX(getX() + deltaPosition);
    setY(getY() + deltaPosition);
    

    float deltaDegrees = ROTATION_DEGREES_PER_SECOND / (1.0F / elapsedTime);
    rotate(deltaDegrees);

    this->updateAnimation(elapsedTime);
}
