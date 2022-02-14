#include "ship.hpp"
#include "shipEventListener.hpp"
#include "bullet.hpp"
#include "direction.hpp"
#include "geo2D.hpp"
#include "vector2D.hpp"
#include "timing.hpp"

const uint16_t Ship::SPEED_IN_PIXELS_PER_SECOND = 200;
const uint32_t Ship::THRUST_FORCE = 24000;
const uint16_t Ship::MASS = 200;
const uint8_t Ship::ROTATION_DEGREES_PER_SECOND = 140;
const uint8_t Ship::SHOTS_PER_SECOND = 10;
const float Ship::SCALE = 0.15F;
const uint16_t Ship::ROTATION_POINT_Y_OFFSET = 125;

Ship::Ship(jimp::GameEngine* gameEngine, ShipEventListener* eventListener) : jimp::AnimatedSprite(gameEngine, gameEngine->getScreenWidth() / 2, gameEngine->getScreenHeight() / 2, SCALE, 0.05F) {
    this->eventListener = eventListener;
    this->firingSound = gameEngine->loadSound("laser.ogg");
    this->thrustSound = gameEngine->loadSound("thrust.ogg");
    addSprite("default", "spaceship.png");
    addSprite("throttling", "spaceship-thrust1.png");
    addSprite("throttling", "spaceship-thrust2.png");
}

void Ship::onUpdate(float elapsedTime) {
    updateFiring(elapsedTime);
    updateMovement(elapsedTime);
    updateRotation(elapsedTime);
    updateSound(elapsedTime);
    
    AnimatedSprite::onUpdate(elapsedTime);
}

void Ship::hasCollidedRect(jimp::AnimatedSprite* otherSprite, jimp::Geo2D::Side side) {
    std::cout << rand() << "ship hit by asteroid" << std::endl;
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
        thrustSound->loop(180);
        setCurrentAnimation("throttling");
    } else {
        isThrustSoundFadingOut = true;
        setCurrentAnimation("default");
    }
}

void Ship::onKeyboardSpaceBar(jimp::KeyState keyState) {
    hasFired = keyState == jimp::KeyState::PRESSED;
    isFiring = keyState == jimp::KeyState::PRESSED;
}

void Ship::updateSound(float elapsedTime) {
    if (isThrustSoundFadingOut && !isThrothling) {
        float volumeDecrease = jimp::Timing::toValueForElapsedTime(200, elapsedTime);
        thrustSound->setVolume(thrustSound->getVolume() - volumeDecrease);
        
        if (thrustSound->getVolume() == 0) {
            isThrustSoundFadingOut = false;
        }
    }
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
        Bullet* bullet = new Bullet(getGameEngine(), getPosition().x + rotationPoint.x - 5, getPosition().y + rotationPoint.y - 10, getRotationAngle());
        firingSound->playTillEnd(30);
        eventListener->onWeaponFired(bullet);
        elapsedTimeSinceLastShot = 0;
    }
}

void Ship::updateMovement(float elapsedTime) {
    if (isThrothling) {
        jimp::Vector2D velocityDelta = jimp::Geo2D::vectorFrom(THRUST_FORCE, getRotationAngle(), MASS, elapsedTime);
        jimp::Vector2D newVelocity = velocity + velocityDelta;
        
        if (abs(newVelocity.x) < SPEED_IN_PIXELS_PER_SECOND || abs(newVelocity.x) < abs(velocity.x)) {
            velocity.x = newVelocity.x;
        }
        if (abs(newVelocity.y) < SPEED_IN_PIXELS_PER_SECOND || abs(newVelocity.y) < abs(velocity.y)) {
            velocity.y = newVelocity.y;
        }
    }
    
    if (isOutsideScreenRight() && velocity.x >= 0) {
        setX(-getWidth());
    } else if (isOutsideScreenLeft() && velocity.x <= 0) {
        setX(getGameEngine()->getScreenWidth());
    }
    if (isOutsideScreenTop() && velocity.y <= 0) {
        setY(getGameEngine()->getScreenHeight());
    } else if (isOutsideScreenBottom() && velocity.y >= 0) {
        setY(-getHeight());
    }
    
    addToPosition(jimp::Timing::toValueForElapsedTime(velocity, elapsedTime));
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
