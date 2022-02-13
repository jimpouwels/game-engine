#include <math.h>
#include "asteroid.hpp"
#include "geo2D.hpp"
#include "timing.hpp"
#include "ship.hpp"

#include <iostream>

const uint16_t Asteroid::FORCE = 180;
const uint16_t Asteroid::ROTATION_DEGREES_PER_SECOND = 50;
const float Asteroid::HIT_ANIMATION_DURATION_IN_SECONDS = 0.1F;

Asteroid::Asteroid(jimp::GameEngine* gameEngine, float x, float y, float directionAngle) : jimp::AnimatedSprite(gameEngine, x, y, 0.1F, -1) {
    addSprite("default", "asteroid.png");
    addSprite("hit", "asteroid-hit.png");
    setRotationAngle(90);
    this->hitSound = new jimp::Sound("hit.ogg");
    this->directionAngle = directionAngle;
}

Asteroid::~Asteroid() {
}

void Asteroid::onUpdate(float elapsedTime) {
    updateDirection(elapsedTime);
    updateMovement(elapsedTime);
    updateRotation(elapsedTime);
    handleHits(elapsedTime);
    
    AnimatedSprite::onUpdate(elapsedTime);
}

void Asteroid::onFrame(float elapsedTime) {
    AnimatedSprite::onFrame(elapsedTime);
}

void Asteroid::hasCollidedRect(jimp::AnimatedSprite *otherSprite, jimp::Geo2D::Side side) {
    if (Bullet* v = dynamic_cast<Bullet*>(otherSprite)) {
        isHit = true;
        this->hitSound->play(14);
        hitCount++;
    } 
}

void Asteroid::hasCollidedRectLeft(jimp::AnimatedSprite* otherSprite) {
    if (Asteroid* v = dynamic_cast<Asteroid*>(otherSprite)) {
        if (directionAngle > 180) {
            directionAngle = jimp::Geo2D::inverseAngleHorizontally(directionAngle);
        }
    }
}

void Asteroid::hasCollidedRectRight(jimp::AnimatedSprite* otherSprite) {
    if (Asteroid* v = dynamic_cast<Asteroid*>(otherSprite)) {
        if (directionAngle < 180) {
            directionAngle = jimp::Geo2D::inverseAngleHorizontally(directionAngle);
        }
    }
}

void Asteroid::hasCollidedRectTop(jimp::AnimatedSprite* otherSprite) {
    if (Asteroid* v = dynamic_cast<Asteroid*>(otherSprite)) {
        if (directionAngle < 90 || directionAngle > 270) {
            directionAngle = jimp::Geo2D::inverseAngleVertically(directionAngle);
        }
    }
}

void Asteroid::hasCollidedRectBottom(jimp::AnimatedSprite* otherSprite) {
    if (Asteroid* v = dynamic_cast<Asteroid*>(otherSprite)) {
        if (directionAngle > 90 && directionAngle < 270) {
            directionAngle = jimp::Geo2D::inverseAngleVertically(directionAngle);
        }
    }
}

void Asteroid::updateDirection(float elapsedTime) {
    if (isEnteringScreen && !isAtLeftEdgeOfScreen() && !isAtRightEdgeOfScreen()) {
        isEnteringScreen = false;
    }
    if (isAtTopEdgeOfScreen() || isAtBottomEdgeOfScreen()) {
        directionAngle = jimp::Geo2D::inverseAngleVertically(directionAngle);
    }
    if (!isEnteringScreen && (isAtLeftEdgeOfScreen() || isAtRightEdgeOfScreen())) {
        directionAngle = jimp::Geo2D::inverseAngleHorizontally(directionAngle);
    }
}

void Asteroid::updateMovement(float elapsedTime) {
    float force = jimp::Timing::toValueForElapsedTime(FORCE, elapsedTime);
    jimp::Vector2D deltaVector = jimp::Geo2D::vectorFrom(directionAngle, force);
    addToPosition(deltaVector);
}

void Asteroid::updateRotation(float elapsedTime) {
    float deltaDegrees = jimp::Timing::toValueForElapsedTime(ROTATION_DEGREES_PER_SECOND, elapsedTime);
    setRotationAngle(getRotationAngle() + deltaDegrees);
}

void Asteroid::handleHits(float elapsedTime) {
    if (hitCount > 5) {
        markForDeletion();
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
