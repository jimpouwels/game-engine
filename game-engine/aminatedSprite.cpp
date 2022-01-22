#include <vector>
#include <map>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "animatedSprite.hpp"
#include "sprite.hpp"
#include "animation.hpp"

namespace jimp {

AnimatedSprite::AnimatedSprite(jimp::GameEngine* gameEngine, float x, float y, float scale, float imageSwapIntervalInSeconds) {
    this->x = x;
    this->y = y;
    this->scale = scale;
    this->gameEngine = gameEngine;
    animationMap = new std::map<std::string, jimp::Animation*>;
    this->imageSwapIntervalInSeconds = imageSwapIntervalInSeconds;
}

AnimatedSprite::AnimatedSprite(jimp::GameEngine* gameEngine, float x, float y, float scale, int rotationAngle, float imageSwapIntervalInSeconds) {
    AnimatedSprite(gameEngine, x, y, scale, 0, imageSwapIntervalInSeconds);
    this->rotationAngle = rotationAngle;
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
    if (elapsedTimeSinceLastSwap >= imageSwapIntervalInSeconds) {
        activeAnimation->switchToNextSprite();
        elapsedTimeSinceLastSwap = 0;
    }
    activeAnimation->getActiveSprite()->setPosition(x, y);
    activeAnimation->getActiveSprite()->setRotationAngle(rotationAngle);
}

float AnimatedSprite::getX() {
    return x;
}

void AnimatedSprite::setX(float x) {
    this->x = x;
}

float AnimatedSprite::getY() {
    return y;
}

void AnimatedSprite::setY(float y) {
    this->y = y;
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

void AnimatedSprite::setRotationAngle(float rotationAngle) {
    this->rotationAngle = rotationAngle;
}

void AnimatedSprite::rotateLeft(float degrees) {
    this->rotationAngle -= degrees;
}

void AnimatedSprite::rotateRight(float degrees) {
    this->rotationAngle += degrees;
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
    jimp::Sprite* sprite = new jimp::Sprite(0, 0, scale, filePath);
    animationMap->find(animationId)->second->addSprite(sprite);
    if (activeAnimation == nullptr) {
        activeAnimation = animation;
        updateAnimation(0);
    }
}

}
