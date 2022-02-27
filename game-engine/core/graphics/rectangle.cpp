#include "rectangle.hpp"

namespace jimp {

Rectangle::Rectangle(int width, int height) : Shape() {
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
