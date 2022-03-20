#include "shape.hpp"

namespace jimp {

Shape::Shape(Color color, float scale, uint16_t repeat) : Drawable(0, 0, scale, 0, repeat) {
    this->color = color;
}

Shape::~Shape() {
}

Color Shape::getColor() {
    return color;
}

int Shape::getWidth() {
    return getSingleWidth() * getRepeat();
}

}
