#include <math.h>
#include "ship.hpp"
#include "bullet.hpp"
#include "geo2D.hpp"
#include "vector2D.hpp"
#include "timing.hpp"
#include "asteroid.hpp"

const uint16_t Ship::SPEED_IN_PIXELS_PER_SECOND = 200;
const uint32_t Ship::THRUST_FORCE = 35000;
const uint16_t Ship::MASS = 200;
const uint8_t Ship::ROTATION_DEGREES_PER_SECOND = 140;
const uint8_t Ship::SHOTS_PER_SECOND = 10;
const float Ship::SCALE = 0.15F;
const uint16_t Ship::ROTATION_POINT_Y_OFFSET = 125;

Ship::Ship() : jimp::Graphic(jimp::Vector2D { .x = 400, .y = 400 }, SCALE, 0, 0.05F) {
    this->firingSound = new jimp::Sound("laser.ogg");
    this->thrustSound = new jimp::Sound("thrust.ogg");
    addDrawable("default", "spaceship.png");
    addDrawable("throttling", "spaceship-thrust1.png");
    addDrawable("throttling", "spaceship-thrust2.png");
    markAsInitialized();
}

void Ship::doOnInit() {
    
}

void Ship::doOnUpdate(float elapsedTime) {
    if (isThrothling) {
        accelerate(getRotationAngle(), MASS, THRUST_FORCE);
    }
    updateFiring(elapsedTime);
    updateMovement(elapsedTime);
    updateRotation(elapsedTime);
}

void Ship::doOnFrame(float elapsedTime) {
}

uint16_t Ship::getZIndex() {
    return 1;
}

void Ship::hasCollidedRect(jimp::Graphic *otherSprite, jimp::Geo2D::Side side) {
    Asteroid* asteroid = dynamic_cast<Asteroid*>(otherSprite);
    if (asteroid != nullptr) {
        
    }
}

void Ship::hasCollidedRectRight(jimp::Graphic* otherSprite) {
    Asteroid* asteroid = dynamic_cast<Asteroid*>(otherSprite);
    if (asteroid != nullptr) {
        
    }
}

void Ship::hasCollidedRectLeft(jimp::Graphic* otherSprite) {
    Asteroid* asteroid = dynamic_cast<Asteroid*>(otherSprite);
    if (asteroid != nullptr) {
       
    }
}

void Ship::hasCollidedRectTop(jimp::Graphic* otherSprite) {
    Asteroid* asteroid = dynamic_cast<Asteroid*>(otherSprite);
    if (asteroid != nullptr) {
        
    }
}

void Ship::hasCollidedRectBottom(jimp::Graphic* otherSprite) {
    Asteroid* asteroid = dynamic_cast<Asteroid*>(otherSprite);
    if (asteroid != nullptr) {
        
    }
}

jimp::Vector2D Ship::getRotationPoint() {
    return jimp::Vector2D { .x = getWidth() / 2.0F, .y = (getHeight() - (ROTATION_POINT_Y_OFFSET * getScale())) / 2.0F };
}

void Ship::onKeyboardLeft(jimp::KeyState keyState) {
    isRotatingLeft = keyState == jimp::KeyState::PRESSED;
}

void Ship::onKeyboardRight(jimp::KeyState keyState) {
    isRotatingRight = keyState == jimp::KeyState::PRESSED;
}

void Ship::onKeyboardUp(jimp::KeyState keyState) {
    isThrothling = keyState == jimp::KeyState::PRESSED;
    if (isThrothling) {
        thrustSound->loop(11);
        setCurrentAnimation("throttling");
    } else {
        thrustSound->fadeOut(1);
        setCurrentAnimation("default");
    }
}

void Ship::onKeyboardSpaceBar(jimp::KeyState keyState) {
    hasFired = keyState == jimp::KeyState::PRESSED;
    isFiring = keyState == jimp::KeyState::PRESSED;
}

void Ship::updateFiring(float elapsedTime) {
    elapsedTimeSinceLastShot += elapsedTime;
    if (!isFiring && !hasFired) {
        return;
    }
    float timeBetweenShots = 1.0F / SHOTS_PER_SECOND;
    if (elapsedTimeSinceLastShot >= timeBetweenShots && (hasFired || isFiring)) {
        hasFired = false;
        jimp::Vector2D rotationPoint = getRotationPoint();
        rotationPoint.x += getPosition().x - 5;
        rotationPoint.y += getPosition().y - 10;
        new Bullet(rotationPoint, getRotationAngle());
        firingSound->playTillEnd(30);
        elapsedTimeSinceLastShot = 0;
    }
}

void Ship::updateMovement(float elapsedTime) {
    if (jimp::GameEngine::getInstance()->isOutsideScreenRight(this) && getVelocity().x >= 0) {
        setX(-getWidth());
    } else if (jimp::GameEngine::getInstance()->isOutsideScreenLeft(this) && getVelocity().x <= 0) {
        setX(jimp::GameEngine::getInstance()->getScreenWidth());
    }
    if (jimp::GameEngine::getInstance()->isOutsideScreenTop(this) && getVelocity().y <= 0) {
        setY(jimp::GameEngine::getInstance()->getScreenHeight());
    } else if (jimp::GameEngine::getInstance()->isOutsideScreenBottom(this) && getVelocity().y >= 0) {
        setY(-getHeight());
    }
}

void Ship::updateRotation(float elapsedTime) {
    if (isRotatingLeft || isRotatingRight) {
        float deltaDegrees = jimp::Timing::toValueForElapsedTime(ROTATION_DEGREES_PER_SECOND, elapsedTime);
        if (isRotatingLeft) {
            deltaDegrees = -deltaDegrees;
        }
        setRotationAngle(jimp::Geo2D::normalizeAngle(getRotationAngle() + deltaDegrees));
    }
}
