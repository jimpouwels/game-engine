#include <vector>
#include <map>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "animatedSprite.hpp"
#include "sprite.hpp"
#include "animation.hpp"

namespace jimp {

AnimatedSprite::AnimatedSprite(GameEngine* gameEngine, float x, float y, float scale, float imageSwapIntervalInSeconds) {
    this->position.x = x;
    this->position.y = y;
    this->scale = scale;
    this->gameEngine = gameEngine;
    animationMap = new std::map<std::string, Animation*>;
    this->imageSwapIntervalInSeconds = imageSwapIntervalInSeconds;
}

AnimatedSprite::AnimatedSprite(GameEngine* gameEngine, float x, float y, float scale, int rotationAngle, float imageSwapIntervalInSeconds) {
    AnimatedSprite(gameEngine, x, y, scale, 0, imageSwapIntervalInSeconds);
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
    }
}

bool AnimatedSprite::checkCollision(AnimatedSprite* otherSprite) {
    Vector2D otherSpritePosition = otherSprite->getPosition();
    bool hasCollided = otherSpritePosition.x > getPosition().x && otherSpritePosition.x < (getPosition().x + getWidth())
        && otherSpritePosition.y > getPosition().y && otherSpritePosition.y < (getPosition().y + getHeight());
    if (hasCollided) {
        hasCollidedWith(otherSprite);
        otherSprite->hasCollidedWith(this);
    }
    return hasCollided;
}

void AnimatedSprite::updateAnimation(float elapsedTime) {
    elapsedTimeSinceLastSwap += elapsedTime;
    if (imageSwapIntervalInSeconds >= 0 && elapsedTimeSinceLastSwap >= imageSwapIntervalInSeconds) {
        activeAnimation->switchToNextSprite();
        elapsedTimeSinceLastSwap = 0;
    }
    Sprite* activeSprite = getActiveSprite();
    activeSprite->setPosition(position);
    activeSprite->setScale(scale);
    activeSprite->setRotationAngle(angle);
    activeSprite->setRotationPoint(getRotationPoint());
}

jimp::GameEngine* AnimatedSprite::getGameEngine() {
    return gameEngine;
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

bool AnimatedSprite::isPositionedWithinScreen() {
    return gameEngine->isPositionWithinScreen(position.x, position.y);
}

float AnimatedSprite::getRotationAngle() {
    return angle;
}

void AnimatedSprite::setRotationAngle(float angle) {
    this->angle = angle;
}

bool AnimatedSprite::isAtLeftEdgeOfScreen() {
    return getActiveSprite()->isAtLeftEdgeOfScreen();
}

bool AnimatedSprite::isAtRightEdgeOfScreen() {
    return getActiveSprite()->isAtRightEdgeOfScreen();
}

bool AnimatedSprite::isAtTopEdgeOfScreen() {
    return getActiveSprite()->isAtTopEdgeOfScreen();
}

bool AnimatedSprite::isAtBottomEdgeOfScreen() {
    return getActiveSprite()->isAtBottomEdgeOfScreen();
}

bool AnimatedSprite::isOutsideScreenTop() {
    return getActiveSprite()->isOutsideScreenTop();
}

bool AnimatedSprite::isOutsideScreenBottom() {
    return getActiveSprite()->isOutsideScreenBottom();
}

bool AnimatedSprite::isOutsideScreenLeft() {
    return getActiveSprite()->isOutsideScreenLeft();
}

bool AnimatedSprite::isOutsideScreenRight() {
    return getActiveSprite()->isOutsideScreenRight();
}

bool AnimatedSprite::isMarkedForDeletion() {
    return markedForDeletion;
}

void AnimatedSprite::addSprite(std::string animationId, std::string filePath) {
    Animation* animation = nullptr;
    if (animationMap->find(animationId) == animationMap->end()) {
        animation = new Animation(gameEngine, animationId);
        animationMap->insert({animationId, animation});
    } else {
        animation = animationMap->find(animationId)->second;
    }
    Sprite* sprite = new Sprite(gameEngine, position.x, position.y, scale, filePath);
    animationMap->find(animationId)->second->addSprite(sprite);
    if (activeAnimation == nullptr) {
        activeAnimation = animation;
        updateAnimation(0);
    }
}

}
