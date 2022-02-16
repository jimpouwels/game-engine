#include <math.h>
#include "asteroid.hpp"
#include "geo2D.hpp"
#include "timing.hpp"
#include "ship.hpp"
#include "mathUtils.hpp"

#include <iostream>

const uint16_t Asteroid::FORCE = 10000;
const float Asteroid::HIT_ANIMATION_DURATION_IN_SECONDS = 0.05F;

Asteroid::Asteroid(jimp::GameEngine* gameEngine, float x, float y, float directionAngle) : jimp::AnimatedSprite(x, y, 0.1F, -1) {
    this->gameEngine = gameEngine;
    addSprite("default", "asteroid.png");
    addSprite("hit", "asteroid-hit.png");
    setRotationAngle(90);
    this->hitSound = gameEngine->registerSound(new jimp::Sound("hit.ogg"));
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
    Asteroid* asteroid = dynamic_cast<Asteroid*>(otherSprite);
    if (asteroid != nullptr) {
        getVelocity().x = -getVelocity().x;
    }
}

void Asteroid::hasCollidedRectRight(jimp::AnimatedSprite* otherSprite) {
    Asteroid* asteroid = dynamic_cast<Asteroid*>(otherSprite);
    if (asteroid != nullptr) {
        getVelocity().x = -getVelocity().x;
    }
}

void Asteroid::hasCollidedRectTop(jimp::AnimatedSprite* otherSprite) {
    Asteroid* asteroid = dynamic_cast<Asteroid*>(otherSprite);
    if (asteroid != nullptr) {
        getVelocity().y = -getVelocity().y;
    }
}

void Asteroid::hasCollidedRectBottom(jimp::AnimatedSprite* otherSprite) {
    Asteroid* asteroid = dynamic_cast<Asteroid*>(otherSprite);
    if (asteroid != nullptr) {
        getVelocity().y = -getVelocity().y;
    }
}

void Asteroid::updateDirection(float elapsedTime) {
    if (isEnteringScreen && !gameEngine->isAtLeftEdgeOfScreen(this) && !gameEngine->isAtRightEdgeOfScreen(this)) {
        isEnteringScreen = false;
    }
    if (gameEngine->isAtTopEdgeOfScreen(this) || gameEngine->isAtBottomEdgeOfScreen(this)) {
        getVelocity().y = -getVelocity().y;
    }
    if (!isEnteringScreen && (gameEngine->isAtLeftEdgeOfScreen(this) || gameEngine->isAtRightEdgeOfScreen(this))) {
        getVelocity().x = -getVelocity().x;
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
