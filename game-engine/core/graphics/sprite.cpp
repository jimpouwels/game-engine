#include "sprite.hpp"
#include "drawable.hpp"

namespace jimp {

Sprite::Sprite(float x, float y, float scale, uint16_t angle, uint16_t repeat, std::string filePath) : Drawable(x, y, scale, angle, repeat) {
    this->image = new Image(filePath);
}

Sprite::~Sprite() {
}

int Sprite::getWidth() {
    return image->getWidth() * getScale() * getRepeat();
}

int Sprite::getSingleWidth() {
    return image->getWidth() * getScale();
}

int Sprite::getHeight() {
    return image->getHeight() * getScale();
}

Image& Sprite::getImage() {
    return *this->image;
}

}
