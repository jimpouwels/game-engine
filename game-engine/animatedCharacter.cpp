#include "animatedCharacter.hpp"
#include "sprite.hpp"
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>

namespace jimp {

AnimatedCharacter::AnimatedCharacter(float x, float y, float imageSwapIntervalInSeconds) {
    this->x = x;
    this->y = y;
    spriteMap = new std::map<std::string, std::vector<jimp::Sprite*>*>;
    this->imageSwapIntervalInSeconds = imageSwapIntervalInSeconds;
}
AnimatedCharacter::~AnimatedCharacter() {
    for (const auto &[collection, imageList]: *spriteMap) {
        for (auto const& image : *imageList) {
            delete image;
        }
        delete imageList;
    }
    delete spriteMap;
}

void AnimatedCharacter::setCurrentSprite(jimp::Sprite* sprite) {
    this->currentSprite = sprite;
}

jimp::Sprite& AnimatedCharacter::getCurrentSprite() {
    currentSprite->setPosition(x, y);
    return *this->currentSprite;
}

void AnimatedCharacter::setNextSpriteInCollection(std::string collection, float elapsedTime) {
    elapsedTimeSinceLastSwap += elapsedTime;
    if (elapsedTimeSinceLastSwap >= imageSwapIntervalInSeconds) {
        currentSpriteIndex++;
        if (collection.compare(currentSpriteCollection) != 0) {
            currentSpriteIndex = 0;
            currentSpriteCollection = collection;
        }
        std::vector<jimp::Sprite*>* spriteList = spriteMap->find(collection)->second;
        if (currentSpriteIndex == spriteList->size()) {
            currentSpriteIndex = 0;
        }
        
        jimp::Sprite* nextSprite = spriteList->at(currentSpriteIndex);
        
        setCurrentSprite(nextSprite);
        
        elapsedTimeSinceLastSwap = 0;
    }
}

void AnimatedCharacter::addSprite(std::string collection, std::string filePath) {
    if (spriteMap->find(collection) == spriteMap->end()) {
        spriteMap->insert({collection, new std::vector<jimp::Sprite*>});
    }
    jimp::Sprite* sprite = new jimp::Sprite(0, 0, filePath);
    spriteMap->find(collection)->second->push_back(sprite);
    if (currentSprite == nullptr) {
        currentSpriteCollection = collection;
        currentSprite = sprite;
    }
}

float AnimatedCharacter::getX() {
    return x;
}

void AnimatedCharacter::setX(float x) {
    this->x = x;
}

float AnimatedCharacter::getY() {
    return y;
}

void AnimatedCharacter::setY(float y) {
    this->y = y;
}

}
