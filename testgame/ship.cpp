#include "ship.hpp"
#include "shipEventListener.hpp"
#include "bullet.hpp"
#include "direction.hpp"
#include "geo2D.hpp"
#include "vector2D.hpp"
#include "timing.hpp"

const int Ship::SPEED_IN_PIXELS_PER_SECOND = 350;
const int Ship::THRUST_FORCE = 25000;
const int Ship::MASS = 4000;
const int Ship::ROTATION_DEGREES_PER_SECOND = 140;
const int Ship::SHOTS_PER_SECOND = 10;
const float Ship::SCALE = 0.2F;

Ship::Ship(jimp::GameEngine* gameEngine, ShipEventListener* eventListener) : jimp::AnimatedSprite(gameEngine, gameEngine->getScreenWidth() / 2, gameEngine->getScreenHeight() / 2, SCALE, 0.05F) {
    this->eventListener = eventListener;
    addSprite("default", "spaceship.png");
    addSprite("throttling", "spaceship-thrust1.png");
    addSprite("throttling", "spaceship-thrust2.png");
}

void Ship::onFrame(float elapsedTime) {
    updateFiring(elapsedTime);
    updateMovement(elapsedTime);
    updateRotation(elapsedTime);
    
    draw(elapsedTime);
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
        setCurrentAnimation("throttling");
    } else {
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
        Bullet* bullet = new Bullet(getGameEngine(), getPosition().x + (getWidth() / 2), getPosition().y + (getHeight() / 2), getRotationAngle());
        eventListener->onWeaponFired(bullet);
        elapsedTimeSinceLastShot = 0;
    }
}

void Ship::updateMovement(float elapsedTime) {
    if (isThrothling) {
        jimp::Vector2D newVelocity = jimp::Geo2D::updateVelocity(velocity, THRUST_FORCE, getRotationAngle(), MASS, elapsedTime);
        
        float maximumVelocity = jimp::Timing::toValueForElapsedTime(SPEED_IN_PIXELS_PER_SECOND, elapsedTime);
        if (abs(newVelocity.x) < maximumVelocity || abs(newVelocity.x) < abs(velocity.x)) {
            velocity.x = newVelocity.x;
        }
        if (abs(newVelocity.y) < maximumVelocity || abs(newVelocity.y) < abs(velocity.y)) {
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
    addToPosition(velocity);
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
