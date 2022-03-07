#include "sprite.hpp"
#include "drawable.hpp"
#include <iostream>

namespace jimp {

Sprite::Sprite(float x, float y, float scale, uint16_t angle, uint16_t repeat, std::string filePath) : Drawable(x, y, scale, angle, repeat) {
    this->image = new Image(filePath);
    crop();
}

Sprite::~Sprite() {
}

int Sprite::getWidth() {
    return (image->getWidth()) * getScale() * getRepeat();
}

int Sprite::getSingleWidth() {
    return (image->getWidth()) * getScale();
}

int Sprite::getHeight() {
    return (image->getHeight()) * getScale();
}

void Sprite::setDrawInversedHorizontally(bool inversed) {
    inversedHorizontally = inversed;
}

bool Sprite::drawInversedHorizontally() {
    return inversedHorizontally;
}

Image& Sprite::getImage() {
    return *this->image;
}

int Sprite::getMarginLeft() {
    return marginLeft;
}

int Sprite::getMarginRight() {
    return marginRight;
}

int Sprite::getMarginTop() {
    return marginTop;
}

int Sprite::getMarginBottom() {
    return marginBottom;
}

void Sprite::crop() {
    bool switchedToRightMargin = false;
    for (int x = 0; x < getImage().getWidth(); x++) {
        bool nonTransparentFound = false;
        for (int y = 0; y < getImage().getHeight(); y++) {
            if (getImage().getImage().getPixel(x, y).a != 0) {
                switchedToRightMargin = true;
                nonTransparentFound = true;
            }
        }
        if (!nonTransparentFound && !switchedToRightMargin) {
            marginLeft++;
        } else if (!nonTransparentFound && switchedToRightMargin) {
            marginRight++;
        }
        nonTransparentFound = false;
        
    }
    bool switchedToBottomMargin = false;
    for (int y = 0; y < getImage().getHeight(); y++) {
        bool nonTransparentFound = false;
        for (int x = 0; x < getImage().getWidth(); x++) {
            if (getImage().getImage().getPixel(x, y).a != 0) {
                switchedToBottomMargin = true;
                nonTransparentFound = true;
            }
        }
        if (!nonTransparentFound && !switchedToBottomMargin) {
            marginTop++;
        } else if (!nonTransparentFound && switchedToBottomMargin) {
            marginBottom++;
        }
        nonTransparentFound = false;
    }
}

}
