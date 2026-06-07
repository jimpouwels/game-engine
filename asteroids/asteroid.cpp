#include <math.h>
#include "asteroid.hpp"
#include "geo2D.hpp"
#include "timing.hpp"
#include "ship.hpp"
#include "mathUtils.hpp"

#include <iostream>

const uint16_t Asteroid::SPEED_IN_PIXELS_PER_SECOND = 150;
const float Asteroid::HIT_ANIMATION_DURATION_IN_SECONDS = 0.05f;

Asteroid::Asteroid(jimp::Vector2D position, float directionAngle) : jimp::AnimatedGraphic() {
    this->setPosition(position);
    this->directionAngle = directionAngle;
    this->rotationDegreesPerSecond = jimp::MathUtils::randomNumberBetween(30, 130);
    this->rotatingDirection = jimp::MathUtils::randomNumberBetween(0, 2) == 1;
    setScale(0.1f);
    setCollidable(true);
    addSprite("default", "asteroid.png");
    addSprite("hit", "asteroid-hit.png");
    setRotationAngle(90);
    this->hitSound = new jimp::Sound("hit.ogg");
    name = "asteroid";
}

Asteroid::~Asteroid() {
}

void Asteroid::doOnUpdate(float elapsedTime) {
    updateDirection(elapsedTime);
    updateRotation(elapsedTime);
    move(directionAngle, SPEED_IN_PIXELS_PER_SECOND);
    animateHit(elapsedTime);
}

void Asteroid::doOnFrame(float elapsedTime) {
}

void Asteroid::hasCollidedRect(jimp::AnimatedGraphic *otherSprite, jimp::Geo2D::Side side) {
    if (dynamic_cast<Bullet*>(otherSprite) != nullptr) {
        isAnimatingHit = true;
        hitCount++;
        this->hitSound->play(14);
        setCurrentAnimation("hit");
        if (hitCount >= 5) {
            markForDeletion();
        }
    } 
}

void Asteroid::hasCollidedRectLeft(jimp::AnimatedGraphic* otherSprite) {
    Asteroid* asteroid = dynamic_cast<Asteroid*>(otherSprite);
    if (asteroid != nullptr) {
        if (directionAngle < 180) {
            directionAngle = jimp::Geo2D::inverseAngleHorizontally(directionAngle);
        }
    }
}

void Asteroid::hasCollidedRectRight(jimp::AnimatedGraphic* otherSprite) {
    Asteroid* asteroid = dynamic_cast<Asteroid*>(otherSprite);
    if (asteroid != nullptr) {
        if (directionAngle > 180) {
            directionAngle = jimp::Geo2D::inverseAngleHorizontally(directionAngle);
        }
    }
}

void Asteroid::hasCollidedRectTop(jimp::AnimatedGraphic* otherSprite) {
    Asteroid* asteroid = dynamic_cast<Asteroid*>(otherSprite);
    if (asteroid != nullptr) {
        if (directionAngle < 90 || directionAngle > 270) {
            directionAngle = jimp::Geo2D::inverseAngleVertically(directionAngle);
        }
    }
}

void Asteroid::hasCollidedRectBottom(jimp::AnimatedGraphic* otherSprite) {
    Asteroid* asteroid = dynamic_cast<Asteroid*>(otherSprite);
    if (asteroid != nullptr) {
        if (directionAngle > 90 && directionAngle < 270) {
            directionAngle = jimp::Geo2D::inverseAngleVertically(directionAngle);
        }
    }
}

void Asteroid::updateDirection(float elapsedTime) {
    if (isEnteringScreen && !jimp::GameEngine::getInstance()->isAtLeftEdgeOfScreen(this) && !jimp::GameEngine::getInstance()->isAtRightEdgeOfScreen(this)) {
        isEnteringScreen = false;
    }
    if (jimp::GameEngine::getInstance()->isAtTopEdgeOfScreen(this) || jimp::GameEngine::getInstance()->isAtBottomEdgeOfScreen(this)) {
        directionAngle = jimp::Geo2D::inverseAngleVertically(directionAngle);
    }
    if (!isEnteringScreen && (jimp::GameEngine::getInstance()->isAtLeftEdgeOfScreen(this) || jimp::GameEngine::getInstance()->isAtRightEdgeOfScreen(this))) {
        directionAngle = jimp::Geo2D::inverseAngleHorizontally(directionAngle);
    }
}

void Asteroid::updateRotation(float elapsedTime) {
    float deltaDegrees = jimp::Timing::toValueForElapsedTime(rotationDegreesPerSecond, elapsedTime);
    if (rotatingDirection) {
        deltaDegrees = -deltaDegrees;
    }
    setRotationAngle(getRotationAngle() + deltaDegrees);
}

void Asteroid::animateHit(float elapsedTime) {
     if (isAnimatingHit) {
        totalHitAnimationDuration += elapsedTime;
        if (totalHitAnimationDuration > HIT_ANIMATION_DURATION_IN_SECONDS) {
            totalHitAnimationDuration = 0.0F;
            setCurrentAnimation("default");
            isAnimatingHit = false;
        }
    }
}
