#include "sprite.hpp"
#include "drawable.hpp"

namespace jimp {

Sprite::Sprite(float x, float y, float scale, uint16_t angle, std::string filePath) : Drawable(x, y, scale, angle) {
    Sprite(x, y, scale, filePath);
}

Sprite::Sprite(float x, float y, float scale, std::string filePath) : Drawable(x, y, scale, 0) {
    this->image = new Image(filePath);
}

Sprite::~Sprite() {
}

int Sprite::getWidth() {
    return image->getWidth() * getScale();
}

int Sprite::getHeight() {
    return image->getHeight() * getScale();
}

Image& Sprite::getImage() {
    return *this->image;
}

}
