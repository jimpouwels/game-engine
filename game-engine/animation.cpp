#include "animation.hpp"

namespace jimp {

Animation::Animation(std::string id) {
    this->id = id;
    this->sprites = new std::vector<Sprite*>;
}

Animation::~Animation() {
    for (auto const& sprite : *sprites) {
        delete sprite;
    }
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
