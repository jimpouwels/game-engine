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

Ship::Ship() : jimp::AnimatedGraphic() {
    setScale(0.2);
    setZIndex(0);
    this->firingSound = new jimp::Sound("laser.ogg");
    this->thrustSound = new jimp::Sound("thrust.ogg");
    addSprite("default", "spaceship.png");
    addSprite("throttling", "spaceship-thrust1.png");
    addSprite("throttling", "spaceship-thrust2.png");
    name = "ship";
}

void Ship::doOnUpdate(float elapsedTime) {
    if (isThrothling) {
        accelerate(getRotationAngle(), MASS, THRUST_FORCE, elapsedTime);
    }
    updateFiring(elapsedTime);
    updateMovement(elapsedTime);
    updateRotation(elapsedTime);
}

void Ship::doOnFrame(float elapsedTime) {
}

void Ship::hasCollidedRect(jimp::AnimatedGraphic *otherSprite, jimp::Geo2D::Side side) {
    Asteroid* asteroid = dynamic_cast<Asteroid*>(otherSprite);
    if (asteroid != nullptr) {
        
    }
}

void Ship::hasCollidedRectRight(jimp::AnimatedGraphic* otherSprite) {
    Asteroid* asteroid = dynamic_cast<Asteroid*>(otherSprite);
    if (asteroid != nullptr) {
        
    }
}

void Ship::hasCollidedRectLeft(jimp::AnimatedGraphic* otherSprite) {
    Asteroid* asteroid = dynamic_cast<Asteroid*>(otherSprite);
    if (asteroid != nullptr) {
       
    }
}

void Ship::hasCollidedRectTop(jimp::AnimatedGraphic* otherSprite) {
    Asteroid* asteroid = dynamic_cast<Asteroid*>(otherSprite);
    if (asteroid != nullptr) {
        
    }
}

void Ship::hasCollidedRectBottom(jimp::AnimatedGraphic* otherSprite) {
    Asteroid* asteroid = dynamic_cast<Asteroid*>(otherSprite);
    if (asteroid != nullptr) {
        
    }
}

jimp::Vector2D Ship::getRotationPoint() {
    return jimp::Vector2D::from(getWidth() / 2.0F, (getHeight() - (ROTATION_POINT_Y_OFFSET * getScale())) / 2.0F);
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
        rotationPoint.x += getPosition().x - (10 * getScale());
        rotationPoint.y += getPosition().y + (45 * getScale());
        jimp::GameEngine::getInstance()->registerGraphic(new Bullet(rotationPoint, getRotationAngle()));
        firingSound->playTillEnd(30);
        elapsedTimeSinceLastShot = 0;
    }
}

void Ship::updateMovement(float elapsedTime) {
    if (jimp::GameEngine::getInstance()->isOutsideScreenRight(this) && getMoveVelocity().x >= 0) {
        setX(-getWidth());
    } else if (jimp::GameEngine::getInstance()->isOutsideScreenLeft(this) && getMoveVelocity().x <= 0) {
        setX(jimp::GameEngine::getInstance()->getScreenWidth());
    }
    if (jimp::GameEngine::getInstance()->isOutsideScreenTop(this) && getMoveVelocity().y <= 0) {
        setY(jimp::GameEngine::getInstance()->getScreenHeight());
    } else if (jimp::GameEngine::getInstance()->isOutsideScreenBottom(this) && getMoveVelocity().y >= 0) {
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
