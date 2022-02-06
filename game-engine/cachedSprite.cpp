#include "cachedSprite.hpp"
#include <SFML/Graphics.hpp>

namespace jimp {

CachedSprite::CachedSprite(sf::Texture* texture, sf::Sprite* sprite) {
    this->texture = texture;
    this->sprite = sprite;
}

CachedSprite::~CachedSprite() {
    delete texture;
    delete sprite;
}

sf::Sprite* CachedSprite::getSprite() {
    return sprite;
}

}
