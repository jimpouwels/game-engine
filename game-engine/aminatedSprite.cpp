#include "animatedSprite.hpp"
#include "sprite.hpp"
#include <vector>
#include <map>
#include <iostream>
#include <SFML/Graphics.hpp>

namespace jimp {

AnimatedSprite::AnimatedSprite(float x, float y, int width, int height, float imageSwapIntervalInSeconds) {
    this->x = x;
    this->y = y;
    spriteMap = new std::map<std::string, std::vector<jimp::Sprite*>*>;
    this->imageSwapIntervalInSeconds = imageSwapIntervalInSeconds;
}

AnimatedSprite::~AnimatedSprite() {
    for (const auto& [animationId, imageList]: *spriteMap) {
        for (auto const& image : *imageList) {
            delete image;
        }
        delete imageList;
    }
    delete spriteMap;
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
    if (spriteMap->find(animationId) == spriteMap->end()) {
        spriteMap->insert({animationId, new std::vector<jimp::Sprite*>});
    }
    jimp::Sprite* sprite = new jimp::Sprite(0, 0, width, height, filePath);
    spriteMap->find(animationId)->second->push_back(sprite);
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
    std::vector<jimp::Sprite*>* spriteList = spriteMap->find(activeAnimationId)->second;
    if (activeSpriteIndex == spriteList->size()) {
        activeSpriteIndex = 0;
    }
    
    jimp::Sprite* nextSprite = spriteList->at(activeSpriteIndex);
    
    setActiveSprite(nextSprite);
}

}
