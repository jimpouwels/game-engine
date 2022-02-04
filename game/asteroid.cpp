#include <math.h>
#include "asteroid.hpp"
#include "screen.hpp"
#include "geo2D.hpp"
#include "timing.hpp"

#include <iostream>

const int Asteroid::SPEED_IN_PIXELS_PER_SECOND = 180;
const int Asteroid::ROTATION_DEGREES_PER_SECOND = 50;
const float Asteroid::HIT_ANIMATION_DURATION_IN_SECONDS = 0.01F;

Asteroid::Asteroid(jimp::Screen* screen, float x, float y, float directionAngle) : jimp::AnimatedSprite(screen, x, y, 0.1F, -1) {
    addSprite("default", "asteroid.png");
    addSprite("hit", "asteroid-hit.png");
    setRotationAngle(90);
    this->directionAngle = directionAngle;
}

Asteroid::~Asteroid() {
}

void Asteroid::onFrame(float elapsedTime) {
    updateDirection(elapsedTime);
    updateMovement(elapsedTime);
    updateRotation(elapsedTime);
    handleHits(elapsedTime);
    
    draw(elapsedTime);
}

bool Asteroid::isHitBy(jimp::AnimatedSprite& animatedSprite) {
    jimp::Vector2D bulletPosition = animatedSprite.getPosition();
    return bulletPosition.x > getPosition().x && bulletPosition.x < (getPosition().x + getWidth())
        && bulletPosition.y > getPosition().y && bulletPosition.y < (getPosition().y + getHeight());
}

void Asteroid::setHit() {
    isHit = true;
    hitCount++;
}

bool Asteroid::isDestroyed() {
    return destroyed;
}

void Asteroid::updateDirection(float elapsedTime) {
    if (getPosition().x > 0 && getPosition().x < (getScreen().getWidth() - getWidth())) {
        isEnteringScreen = false;
    }
    if (!isEnteringScreen) {
        if (isOutsideScreenAbove() || isOutsideScreenBelow()) {
            directionAngle = jimp::Geo2D::inverseAngleVertically(directionAngle);
        }
        if (isOutsideScreenLeft() || isOutsideScreenRight()) {
            directionAngle = jimp::Geo2D::inverseAngleHorizontally(directionAngle);
        }
    }
}

void Asteroid::updateMovement(float elapsedTime) {
    float distance = jimp::Timing::toValueForElapsedTime(SPEED_IN_PIXELS_PER_SECOND, elapsedTime);
    jimp::Vector2D deltaVector = jimp::Geo2D::deltaVectorByAngle(directionAngle, distance);
    addToPosition(deltaVector);
}

void Asteroid::updateRotation(float elapsedTime) {
    float deltaDegrees = jimp::Timing::toValueForElapsedTime(ROTATION_DEGREES_PER_SECOND, elapsedTime);
    setRotationAngle(getRotationAngle() + deltaDegrees);
}

void Asteroid::handleHits(float elapsedTime) {
    if (hitCount > 5) {
        destroyed = true;
    } else if (isHit) {
        setCurrentAnimation("hit");
        totalHitAnimationDuration += elapsedTime;
        if (totalHitAnimationDuration > HIT_ANIMATION_DURATION_IN_SECONDS) {
            totalHitAnimationDuration = 0.0F;
            isHit = false;
        }
    } else {
        setCurrentAnimation("default");
    }
}
