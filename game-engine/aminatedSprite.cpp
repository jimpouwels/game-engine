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
    for (const auto& [collection, imageList]: *spriteMap) {
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

void AnimatedSprite::switchToNextSprite(std::string collection, float elapsedTime) {
    elapsedTimeSinceLastSwap += elapsedTime;
    if (activeCollection.compare(collection) != 0) {
        activeCollection = collection;
        activeSpriteIndex = 0;
        updateActiveSprite();
    }
    if (elapsedTimeSinceLastSwap >= imageSwapIntervalInSeconds) {
        activeSpriteIndex++;
        updateActiveSprite();
        elapsedTimeSinceLastSwap = 0;
    }
}

void AnimatedSprite::addSprite(std::string collection, std::string filePath) {
    if (spriteMap->find(collection) == spriteMap->end()) {
        spriteMap->insert({collection, new std::vector<jimp::Sprite*>});
    }
    jimp::Sprite* sprite = new jimp::Sprite(0, 0, width, height, filePath);
    spriteMap->find(collection)->second->push_back(sprite);
    if (activeSprite == nullptr) {
        activeCollection = collection;
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
    std::vector<jimp::Sprite*>* spriteList = spriteMap->find(activeCollection)->second;
    if (activeSpriteIndex == spriteList->size()) {
        activeSpriteIndex = 0;
    }
    
    jimp::Sprite* nextSprite = spriteList->at(activeSpriteIndex);
    
    setActiveSprite(nextSprite);
}

}
