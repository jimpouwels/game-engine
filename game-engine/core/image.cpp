#include "image.hpp"

namespace jimp {

Image::Image(std::string filePath) {
    loadImage(filePath);
    this->filePath = filePath;
}

Image::~Image() {
    delete image;
}

uint16_t Image::getWidth() {
    return image->getSize().x;
}

uint16_t Image::getHeight() {
    return image->getSize().y;
}

void Image::loadImage(std::string filePath) {
    this->image = new sf::Image();
    image->loadFromFile(filePath);
}

sf::Image& Image::getImage() {
    return *image;
}

std::string Image::getFilePath() {
    return filePath;
}

}
