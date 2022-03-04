#include "shape.hpp"

namespace jimp {

Shape::Shape(uint32_t color, uint16_t repeat) : Drawable(0, 0, 0, 0, repeat) {
    this->color = color;
}

Shape::~Shape() {
}

uint32_t Shape::getColor() {
    return color;
}

int Shape::getWidth() {
    return getSingleWidth() * getRepeat();
}

}
