#include "rectangle.hpp"

namespace jimp {

Rectangle::Rectangle(int width, int height, uint32_t color) : Shape(color) {
    this->width = width;
    this->height = height;
}

int Rectangle::getWidth() {
    return width * getScale();
}

int Rectangle::getHeight() {
    return height * getScale();
}

}
