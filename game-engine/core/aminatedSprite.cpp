#include <vector>
#include <map>
#include <list>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "animatedSprite.hpp"
#include "sprite.hpp"
#include "animation.hpp"
#include "mathUtils.hpp"

namespace jimp {

AnimatedSprite::AnimatedSprite(float x, float y, float scale, float imageSwapIntervalInSeconds) {
    this->position.x = x;
    this->position.y = y;
    this->scale = scale;
    animationMap = new std::map<std::string, Animation*>;
    this->imageSwapIntervalInSeconds = imageSwapIntervalInSeconds;
}

AnimatedSprite::AnimatedSprite(float x, float y, float scale, int rotationAngle, float imageSwapIntervalInSeconds) {
    AnimatedSprite(x, y, scale, 0, imageSwapIntervalInSeconds);
    this->angle = rotationAngle;
}

AnimatedSprite::~AnimatedSprite() {
    for (const auto& [animationId, animation]: *animationMap) {
        delete animation;
    }
    delete animationMap;
}

Sprite* AnimatedSprite::getActiveSprite() {
    return activeAnimation->getActiveSprite();
}

void AnimatedSprite::setCurrentAnimation(std::string animationId) {
    if (activeAnimation->getId().compare(animationId) != 0) {
        activeAnimation = animationMap->find(animationId)->second;
        updateCurrentSpriteData();
    }
}

bool AnimatedSprite::checkCollisionRect(AnimatedSprite* otherSprite) {
    Vector2D otherSpritePosition = otherSprite->getPosition();
    
    if (abs((getPosition().x + getWidth()) - otherSpritePosition.x) <= 2 &&
        (((jimp::MathUtils::isBetween(getPosition().y, otherSpritePosition.y, otherSpritePosition.y + otherSprite->getHeight())) ||
         ((jimp::MathUtils::isBetween(getPosition().y + getHeight(), otherSprite->getPosition().y, otherSprite->getPosition().y + otherSprite->getHeight())))) ||
        ((jimp::MathUtils::isBetween(otherSpritePosition.y, getPosition().y, getPosition().y + getHeight())) ||
         ((jimp::MathUtils::isBetween(otherSpritePosition.y + otherSprite->getHeight(), getPosition().y, getPosition().y + getHeight())))))) {
        hasCollidedRect(otherSprite, Geo2D::Side::RIGHT);
        hasCollidedRectRight(otherSprite);
        otherSprite->hasCollidedRect(this, Geo2D::Side::LEFT);
        otherSprite->hasCollidedRectLeft(this);
        return true;
    } else if (abs(getPosition().x - (otherSpritePosition.x + otherSprite->getWidth())) <= 2 &&
            (((jimp::MathUtils::isBetween(otherSpritePosition.y, getPosition().y, getPosition().y + getHeight())) ||
             (jimp::MathUtils::isBetween(otherSpritePosition.y + otherSprite->getHeight(), getPosition().y, getPosition().y + getHeight()))) ||
               ((jimp::MathUtils::isBetween(getPosition().y, otherSpritePosition.y, otherSpritePosition.y + otherSprite->getHeight())) ||
                (jimp::MathUtils::isBetween(getPosition().y + getHeight(), otherSpritePosition.y, otherSpritePosition.y + otherSprite->getHeight()))))) {
        hasCollidedRect(otherSprite, Geo2D::Side::LEFT);
        hasCollidedRectLeft(otherSprite);
        otherSprite->hasCollidedRect(this, Geo2D::Side::RIGHT);
        otherSprite->hasCollidedRectRight(this);
        return true;
    } else if (abs((getPosition().y + getHeight()) - otherSpritePosition.y) <= 2 &&
            (((jimp::MathUtils::isBetween(getPosition().x, otherSpritePosition.x, otherSpritePosition.x + otherSprite->getWidth()) ||
             (jimp::MathUtils::isBetween(getPosition().x + getWidth(), otherSpritePosition.x, otherSpritePosition.x + otherSprite->getWidth())))) ||
               ((jimp::MathUtils::isBetween(otherSpritePosition.x, getPosition().x, getPosition().x + getWidth()) ||
                (jimp::MathUtils::isBetween(otherSpritePosition.x + otherSprite->getWidth(), getPosition().x, getPosition().x + getWidth())))))) {
        hasCollidedRect(otherSprite, Geo2D::Side::BOTTOM);
        hasCollidedRectBottom(otherSprite);
        otherSprite->hasCollidedRect(this, Geo2D::Side::TOP);
        otherSprite->hasCollidedRectTop(this);
        return true;
    } else if (abs(getPosition().y - (otherSpritePosition.y + otherSprite->getHeight())) <= 2 &&
            (((jimp::MathUtils::isBetween(otherSpritePosition.x, getPosition().x, getPosition().x + getWidth()) ||
             (jimp::MathUtils::isBetween(otherSpritePosition.x + otherSprite->getWidth(), getPosition().x, getPosition().x + getWidth())))) ||
               ((jimp::MathUtils::isBetween(getPosition().x, otherSpritePosition.x, otherSpritePosition.x + otherSprite->getWidth()) ||
                (jimp::MathUtils::isBetween(getPosition().x + getWidth(), otherSpritePosition.x, otherSpritePosition.x + otherSprite->getWidth())))))) {
        hasCollidedRect(otherSprite, Geo2D::Side::TOP);
        hasCollidedRectTop(otherSprite);
        otherSprite->hasCollidedRect(this, Geo2D::Side::BOTTOM);
        otherSprite->hasCollidedRectBottom(this);
        return true;
    }
    
    return false;
}

void AnimatedSprite::updateAnimation(float elapsedTime) {
    elapsedTimeSinceLastSwap += elapsedTime;
    if (imageSwapIntervalInSeconds >= 0 && elapsedTimeSinceLastSwap >= imageSwapIntervalInSeconds) {
        activeAnimation->switchToNextSprite();
        elapsedTimeSinceLastSwap = 0;
    }
    updateCurrentSpriteData();
}

void AnimatedSprite::setX(float x) {
    position.x = x;
}

void AnimatedSprite::setY(float y) {
    position.y = y;
}

Vector2D& AnimatedSprite::getPosition() {
    return position;
}

Vector2D& AnimatedSprite::getVelocity() {
    return velocity;
}

void AnimatedSprite::setPosition(Vector2D position) {
    this->position = position;
}

void AnimatedSprite::addToPosition(Vector2D delta) {
    position.x += delta.x;
    position.y += delta.y;
}

int AnimatedSprite::getWidth() {
    return getActiveSprite()->getWidth();
}

int AnimatedSprite::getHeight() {
    return getActiveSprite()->getHeight();
}

float AnimatedSprite::getScale() {
    return scale;
}

float AnimatedSprite::getRotationAngle() {
    return angle;
}

std::list<Sprite*> AnimatedSprite::getAllSprites() {
    std::list<Sprite*> allSprites = std::list<Sprite*>();
    for (const auto& [animationId, animation]: *animationMap) {
        for (const auto& sprite: animation->getAllSprites()) {
            allSprites.push_back(sprite);
        }
    }
    return allSprites;
}

void AnimatedSprite::accelerate(float angle, uint16_t mass, uint16_t force) {
    this->isAccelerating = true;
    this->velocityAngle = jimp::Geo2D::normalizeAngle(angle);
    this->mass = mass;
    this->moveForce = force;
}

void AnimatedSprite::setRotationAngle(float angle) {
    this->angle = angle;
}

bool AnimatedSprite::isMarkedForDeletion() {
    return markedForDeletion;
}

uint16_t AnimatedSprite::getVelocityAngle() {
    return velocityAngle;
}

void AnimatedSprite::updateMovement(float elapsedTime) {
    if (isAccelerating) {
        velocity = velocity + jimp::Geo2D::vectorFrom(moveForce, velocityAngle, mass, elapsedTime);
        isAccelerating = false;
    }
    addToPosition(jimp::Timing::toValueForElapsedTime(velocity, elapsedTime));
}

void AnimatedSprite::updateCurrentSpriteData() {
    Sprite* activeSprite = getActiveSprite();
    activeSprite->setPosition(position);
    activeSprite->setScale(scale);
    activeSprite->setRotationAngle(angle);
    activeSprite->setRotationPoint(getRotationPoint());
}

void AnimatedSprite::addSprite(std::string animationId, std::string filePath) {
    Animation* animation = nullptr;
    if (animationMap->find(animationId) == animationMap->end()) {
        animation = new Animation(animationId);
        animationMap->insert({animationId, animation});
    } else {
        animation = animationMap->find(animationId)->second;
    }
    Sprite* sprite = new Sprite(position.x, position.y, scale, filePath);
    animationMap->find(animationId)->second->addSprite(sprite);
    if (activeAnimation == nullptr) {
        activeAnimation = animation;
        updateAnimation(0);
    }
}

}
