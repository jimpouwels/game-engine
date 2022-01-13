#include <vector>
#include <map>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "animatedSprite.hpp"
#include "sprite.hpp"
#include "animation.hpp"

namespace jimp {

AnimatedSprite::AnimatedSprite(float x, float y, int width, int height, float imageSwapIntervalInSeconds) {
    this->x = x;
    this->y = y;
    animationMap = new std::map<std::string, jimp::Animation*>;
    this->imageSwapIntervalInSeconds = imageSwapIntervalInSeconds;
}

AnimatedSprite::~AnimatedSprite() {
    for (const auto& [animationId, animation]: *animationMap) {
        delete animation;
    }
    delete animationMap;
}

jimp::Sprite& AnimatedSprite::getActiveSprite() {
    jimp::Sprite* activeSprite = activeAnimation->getActiveSprite();
    activeSprite->setPosition(x, y);
    return *activeSprite;
}

void AnimatedSprite::setCurrentAnimation(std::string animationId) {
    if (activeAnimation->getId().compare(animationId) != 0) {
        activeAnimation = animationMap->find(animationId)->second;
    }
}

void AnimatedSprite::switchToNextSprite(float elapsedTime) {
    elapsedTimeSinceLastSwap += elapsedTime;
    if (elapsedTimeSinceLastSwap >= imageSwapIntervalInSeconds) {
        activeAnimation->switchToNextSprite();
        elapsedTimeSinceLastSwap = 0;
    }
}

void AnimatedSprite::addSprite(std::string animationId, std::string filePath) {
    jimp::Animation* newAnimation = new jimp::Animation(animationId);
    if (animationMap->find(animationId) == animationMap->end()) {
        animationMap->insert({animationId, newAnimation});
    }
    jimp::Sprite* sprite = new jimp::Sprite(0, 0, filePath);
    animationMap->find(animationId)->second->addSprite(sprite);
    if (activeAnimation == nullptr) {
        activeAnimation = newAnimation;
    }
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

}
