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

void AnimatedSprite::draw(float elapsedTime) {
    updateAnimation(elapsedTime);
    gameEngine->draw(getActiveSprite());
}

Sprite* AnimatedSprite::getActiveSprite() {
    Sprite* activeSprite = activeAnimation->getActiveSprite();
    activeSprite->setPosition(position);
    activeSprite->setScale(scale);
    activeSprite->setRotationAngle(angle);
    return activeAnimation->getActiveSprite();
}

void AnimatedSprite::setCurrentAnimation(std::string animationId) {
    if (activeAnimation->getId().compare(animationId) != 0) {
        activeAnimation = animationMap->find(animationId)->second;
    }
}

void AnimatedSprite::updateAnimation(float elapsedTime) {
    elapsedTimeSinceLastSwap += elapsedTime;
    if (elapsedTimeSinceLastSwap >= imageSwapIntervalInSeconds) {
        activeAnimation->switchToNextSprite();
        elapsedTimeSinceLastSwap = 0;
    }
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
    return activeAnimation->getActiveSprite()->getWidth();
}

int AnimatedSprite::getHeight() {
    return activeAnimation->getActiveSprite()->getHeight();
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

bool AnimatedSprite::isOutsideScreenLeft() {
    return getActiveSprite()->isOutsideScreenLeft();
}

bool AnimatedSprite::isOutsideScreenRight() {
    return getActiveSprite()->isOutsideScreenRight();
}

bool AnimatedSprite::isOutsideScreenAbove() {
    return getActiveSprite()->isOutsideScreenAbove();
}

bool AnimatedSprite::isOutsideScreenBelow() {
    return getActiveSprite()->isOutsideScreenBelow();
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
