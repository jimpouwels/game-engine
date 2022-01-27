#include <vector>
#include <map>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "animatedSprite.hpp"
#include "sprite.hpp"
#include "animation.hpp"

namespace jimp {

AnimatedSprite::AnimatedSprite(jimp::GameEngine* gameEngine, float x, float y, float scale, float imageSwapIntervalInSeconds) {
    this->gameEngine = gameEngine;
    this->x = x;
    this->y = y;
    this->scale = scale;
    animationMap = new std::map<std::string, jimp::Animation*>;
    this->imageSwapIntervalInSeconds = imageSwapIntervalInSeconds;
}

AnimatedSprite::AnimatedSprite(jimp::GameEngine* gameEngine, float x, float y, float scale, int rotationAngle, float imageSwapIntervalInSeconds) {
    AnimatedSprite(gameEngine, x, y, scale, 0, imageSwapIntervalInSeconds);
}

AnimatedSprite::~AnimatedSprite() {
    for (const auto& [animationId, animation]: *animationMap) {
        delete animation;
    }
    delete animationMap;
}

jimp::Sprite& AnimatedSprite::getActiveSprite() {
    return *activeAnimation->getActiveSprite();
}

void AnimatedSprite::setCurrentAnimation(std::string animationId) {
    if (activeAnimation->getId().compare(animationId) != 0) {
        activeAnimation = animationMap->find(animationId)->second;
    }
}

void AnimatedSprite::updateAnimation(float elapsedTime) {
    elapsedTimeSinceLastSwap += elapsedTime;
    float rotationAngle = getActiveSprite().getRotationAngle();
    float x = getActiveSprite().getX();
    float y = getActiveSprite().getY();
    if (elapsedTimeSinceLastSwap >= imageSwapIntervalInSeconds) {
        activeAnimation->switchToNextSprite();
        elapsedTimeSinceLastSwap = 0;
    }
    activeAnimation->getActiveSprite()->setPosition(x, y);
    activeAnimation->getActiveSprite()->setRotationAngle(rotationAngle);
}

float AnimatedSprite::getX() {
    return getActiveSprite().getX();
}

void AnimatedSprite::setX(float x) {
    getActiveSprite().setX(x);
}

float AnimatedSprite::getY() {
    return getActiveSprite().getY();
}

void AnimatedSprite::setY(float y) {
    getActiveSprite().setY(y);
}

int AnimatedSprite::getWidth() {
    return activeAnimation->getActiveSprite()->getWidth();
}

int AnimatedSprite::getHeight() {
    return activeAnimation->getActiveSprite()->getHeight();
}

bool AnimatedSprite::isPositionedWithinScreen() {
    return gameEngine->isPositionWithinScreen(getX(), getY());
}

void AnimatedSprite::rotate(float angle) {
    getActiveSprite().rotate(angle);
}

float AnimatedSprite::getRotationAngle() {
    return getActiveSprite().getRotationAngle();
}

void AnimatedSprite::setRotationAngle(float angle) {
    getActiveSprite().setRotationAngle(angle);
}

GameEngine& AnimatedSprite::getGameEngine() {
    return *gameEngine;
}

void AnimatedSprite::addSprite(std::string animationId, std::string filePath) {
    jimp::Animation* animation = nullptr;
    if (animationMap->find(animationId) == animationMap->end()) {
        animation = new jimp::Animation(animationId);
        animationMap->insert({animationId, animation});
    } else {
        animation = animationMap->find(animationId)->second;
    }
    jimp::Sprite* sprite = new jimp::Sprite(x, y, scale, filePath);
    animationMap->find(animationId)->second->addSprite(sprite);
    if (activeAnimation == nullptr) {
        activeAnimation = animation;
        updateAnimation(0);
    }
}

}
