#include <iostream>
#include "gamingInterface.hpp"

namespace jimp {

GamingInterface::GamingInterface(Screen* screen) {
    this->screen = screen;
    this->imageCache = new std::map<std::string, Image*>;
}

Screen* GamingInterface::getScreen() {
    return screen;
}

Image* GamingInterface::loadImage(std::string filePath) {
    Image* image = nullptr;
    if (imageCache->find(filePath) == imageCache->end()) {
        image = new Image(filePath);
        imageCache->insert({filePath, image});
    } else {
        image = imageCache->find(filePath)->second;
    }
    return image;
}

}
