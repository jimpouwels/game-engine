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

long Animation::getNumberOfSprites() {
    return sprites->size();
}

Sprite* Animation::getSpriteAt(int index) {
    return sprites->at(index);
}

}
