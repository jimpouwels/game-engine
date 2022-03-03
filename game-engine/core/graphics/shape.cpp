#include "shape.hpp"

namespace jimp {

Shape::Shape(uint32_t color) : Drawable(0, 0, 0, 0, 0) {
    this->color = color;
}

Shape::~Shape() {
}

uint32_t Shape::getColor() {
    return color;
}

}
