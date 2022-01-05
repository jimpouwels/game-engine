#include "animatedCharacter.h"

namespace jimp {

AnimatedCharacter::AnimatedCharacter(Sprite* sprite) {
    this->sprite = sprite;
}

AnimatedCharacter::~AnimatedCharacter() {
    delete this->sprite;
}

std::string AnimatedCharacter::getFilePath() {
    return this->sprite->getFilePath();
}

int AnimatedCharacter::getX() {
    return this->sprite->getX();
}

int AnimatedCharacter::getY() {
    return this->sprite->getY();
}

}
