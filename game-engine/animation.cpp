#include "animation.hpp"

namespace jimp {

Animation::Animation(GameEngine* gameEngine, std::string id) {
    this->id = id;
    this->sprites = new std::vector<Sprite*>;
    this->gameEngine = gameEngine;
}

Animation::~Animation() {
    for (auto const& sprite : *sprites) {
        gameEngine->eraseFromCache(sprite);
        delete sprite;
    }
    delete sprites;
}

void Animation::addSprite(Sprite *sprite) {
    sprites->push_back(sprite);
}

std::string Animation::getId() {
    return id;
}

long Animation::getNumberOfSprites() {
    return sprites->size();
}

Sprite* Animation::getActiveSprite() {
    return sprites->at(activeSpriteIndex);;
}

void Animation::switchToNextSprite() {
    if (sprites->size() > 1) {
        activeSpriteIndex++;
        if (activeSpriteIndex == getNumberOfSprites()) {
            activeSpriteIndex = 0;
        }
    }
}

}
