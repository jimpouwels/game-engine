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
    activeSprite->setPosition(x, y);
    return *this->activeSprite;
}

void AnimatedSprite::setCurrentAnimation(std::string animationId) {
    if (activeAnimationId.compare(animationId) != 0) {
        activeAnimationId = animationId;
        activeSpriteIndex = 0;
        updateActiveSprite();
    }
}

void AnimatedSprite::switchToNextSprite(float elapsedTime) {
    elapsedTimeSinceLastSwap += elapsedTime;
    if (elapsedTimeSinceLastSwap >= imageSwapIntervalInSeconds) {
        activeSpriteIndex++;
        updateActiveSprite();
        elapsedTimeSinceLastSwap = 0;
    }
}

void AnimatedSprite::addSprite(std::string animationId, std::string filePath) {
    if (animationMap->find(animationId) == animationMap->end()) {
        animationMap->insert({animationId, new jimp::Animation(animationId)});
    }
    jimp::Sprite* sprite = new jimp::Sprite(0, 0, filePath);
    animationMap->find(animationId)->second->addSprite(sprite);
    if (activeSprite == nullptr) {
        activeAnimationId = animationId;
        activeSprite = sprite;
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

void AnimatedSprite::setActiveSprite(jimp::Sprite* sprite) {
    this->activeSprite = sprite;
}

void AnimatedSprite::updateActiveSprite() {
    jimp::Animation* animation = animationMap->find(activeAnimationId)->second;
    if (activeSpriteIndex == animation->getNumberOfSprites()) {
        activeSpriteIndex = 0;
    }
    jimp::Sprite* nextSprite = animation->getSpriteAt(activeSpriteIndex);
    setActiveSprite(nextSprite);
}

}
