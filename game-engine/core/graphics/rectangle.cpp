#include "rectangle.hpp"

namespace jimp {

Rectangle::Rectangle(int width, int height, float scale, Color color, uint16_t repeat) : Shape(color, scale, repeat) {
    this->width = width;
    this->height = height;
}

int Rectangle::getSingleWidth() {
    return width * getScale();
}

int Rectangle::getHeight() {
    return height * getScale();
}

}
