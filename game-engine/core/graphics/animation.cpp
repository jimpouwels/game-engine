#include "animation.hpp"

namespace jimp {

Animation::Animation(std::string id) {
    this->id = id;
    this->drawables = new std::vector<Drawable*>;
}

Animation::~Animation() {
    for (auto const& drawable : *drawables) {
        delete drawable;
    }
    delete drawables;
}

void Animation::addDrawable(Drawable *drawable) {
    drawables->push_back(drawable);
}

std::string Animation::getId() {
    return id;
}

void Animation::setSpriteSwapInterval(float swapInterval) {
    this->spriteSwapIntervalInSeconds = swapInterval;
}

float Animation::getSpriteSwapInterval() {
    return spriteSwapIntervalInSeconds;
}

long Animation::getNumberOfDrawables() {
    return drawables->size();
}

Drawable* Animation::getActiveDrawable() {
    return drawables->at(activeDrawableIndex);;
}

std::list<Drawable*> Animation::getAllDrawables() {
    std::list<Drawable*> allDrawables = std::list<Drawable*>();
    std::copy(drawables->begin(), drawables->end(), std::back_inserter(allDrawables));
    return allDrawables;
}

void Animation::switchToNextDrawable() {
    if (drawables->size() > 1) {
        activeDrawableIndex++;
        if (activeDrawableIndex == getNumberOfDrawables()) {
            activeDrawableIndex = 0;
        }
    }
}

}
