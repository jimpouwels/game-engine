#include "image.hpp"

namespace jimp {

Image::Image(std::string filePath) {
    loadImage(filePath);
}

Image::~Image() {
    delete image;
}

int Image::getWidth() {
    return image->getSize().x;
}

int Image::getHeight() {
    return image->getSize().y;
}

void Image::loadImage(std::string filePath) {
    this->filePath = filePath;
    this->image = new sf::Image();
    image->loadFromFile(filePath);
}

sf::Image& Image::getImage() {
    return *image;
}

}
