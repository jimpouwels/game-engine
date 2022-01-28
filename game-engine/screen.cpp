#include "screen.hpp"
#include "gameEngine.hpp"

namespace jimp {

Screen::Screen(GameEngine* gameEngine) {
    this->gameEngine = gameEngine;
}

Screen::~Screen() {
}

int Screen::getWidth() {
    return gameEngine->getScreenWidth();
}

int Screen::getHeight() {
    return gameEngine->getScreenHeight();
}

bool Screen::isWithin(float x, float y) {
    return gameEngine->isPositionWithinScreen(x, y);
}

}
