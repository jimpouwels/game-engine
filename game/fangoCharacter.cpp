#include "fangoCharacter.hpp"
#include "animatedCharacter.hpp"
#include "sprite.hpp"
#include <iostream>

namespace jimp {

FangoCharacter::FangoCharacter() : jimp::AnimatedCharacter(new jimp::Sprite(0, 0, "fango.png")) {
}

void FangoCharacter::onKeyboardLeft(KeyState keyState) {
    std::cout << "Left " << keyState << std::endl;
}

void FangoCharacter::onKeyboardRight(KeyState keyState) {
    std::cout << "Right " << keyState << std::endl;
}

void FangoCharacter::onKeyboardUp(KeyState keyState) {
    std::cout << "Up " << keyState << std::endl;
}

void FangoCharacter::onKeyboardDown(KeyState keyState) {
    std::cout << "Bottom " << keyState << std::endl;
}

}
