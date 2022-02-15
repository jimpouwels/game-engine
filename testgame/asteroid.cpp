#include <math.h>
#include "asteroid.hpp"
#include "geo2D.hpp"
#include "timing.hpp"
#include "ship.hpp"
#include "mathUtils.hpp"

#include <iostream>

const uint16_t Asteroid::FORCE = 60000;
const float Asteroid::HIT_ANIMATION_DURATION_IN_SECONDS = 0.05F;

Asteroid::Asteroid(jimp::GameEngine* gameEngine, float x, float y, float directionAngle) : jimp::AnimatedSprite(gameEngine, x, y, 0.1F, -1) {
    addSprite("default", "asteroid.png");
    addSprite("hit", "asteroid-hit.png");
    setRotationAngle(90);
    this->hitSound = new jimp::Sound(gameEngine, "hit.ogg");
    this->rotationDegreesPerSecond = jimp::MathUtils::randomNumberBetween(30, 130);
    this->rotatingDirection = jimp::MathUtils::randomNumberBetween(0, 2) == 1;
    accelerate(directionAngle, 0, FORCE);
}

Asteroid::~Asteroid() {
}

void Asteroid::onUpdate(float elapsedTime) {
    updateDirection(elapsedTime);
    updateRotation(elapsedTime);
    animateHit(elapsedTime);
    
    AnimatedSprite::onUpdate(elapsedTime);
}

void Asteroid::onFrame(float elapsedTime) {
    AnimatedSprite::onFrame(elapsedTime);
}

void Asteroid::hasCollidedRect(jimp::AnimatedSprite *otherSprite, jimp::Geo2D::Side side) {
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

void Asteroid::hasCollidedRectLeft(jimp::AnimatedSprite* otherSprite) {
    if (dynamic_cast<Asteroid*>(otherSprite) != nullptr) {
        if (getVelocityAngle() > 180) {
            updateVelocityAngle(jimp::Geo2D::inverseAngleHorizontally(getVelocityAngle()));
        }
    }
}

void Asteroid::hasCollidedRectRight(jimp::AnimatedSprite* otherSprite) {
    if (dynamic_cast<Asteroid*>(otherSprite) != nullptr) {
        if (getVelocityAngle() < 180) {
            updateVelocityAngle(jimp::Geo2D::inverseAngleHorizontally(getVelocityAngle()));
        }
    }
}

void Asteroid::hasCollidedRectTop(jimp::AnimatedSprite* otherSprite) {
    if (dynamic_cast<Asteroid*>(otherSprite) != nullptr) {
        if (getVelocityAngle() < 90 || getVelocityAngle() > 270) {
            updateVelocityAngle(jimp::Geo2D::inverseAngleVertically(getVelocityAngle()));
        }
    }
}

void Asteroid::hasCollidedRectBottom(jimp::AnimatedSprite* otherSprite) {
    if (dynamic_cast<Asteroid*>(otherSprite) != nullptr) {
        if (getVelocityAngle() > 90 && getVelocityAngle() < 270) {
            updateVelocityAngle(jimp::Geo2D::inverseAngleVertically(getVelocityAngle()));
        }
    }
}

jimp::Vector2D Asteroid::getVelocity() {
    return velocity;
}

void Asteroid::updateDirection(float elapsedTime) {
    if (isEnteringScreen && !isAtLeftEdgeOfScreen() && !isAtRightEdgeOfScreen()) {
        isEnteringScreen = false;
    }
    if (isAtTopEdgeOfScreen() || isAtBottomEdgeOfScreen()) {
        updateVelocityAngle(jimp::Geo2D::inverseAngleVertically(getVelocityAngle()));
    }
    if (!isEnteringScreen && (isAtLeftEdgeOfScreen() || isAtRightEdgeOfScreen())) {
        updateVelocityAngle(jimp::Geo2D::inverseAngleHorizontally(getVelocityAngle()));
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
