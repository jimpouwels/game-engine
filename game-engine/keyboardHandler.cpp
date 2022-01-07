#include <list>
#include "keyboardHandler.hpp"
#include "keyListener.hpp"

namespace jimp {

KeyboardHandler::KeyboardHandler() {
    keyListeners = new std::list<jimp::KeyListener*>();
}

KeyboardHandler::~KeyboardHandler() {
    for (auto const& item : *keyListeners) {
        delete item;
    }
    delete keyListeners;
}

void KeyboardHandler::handleEvent(sf::Event event) {
    
}

void KeyboardHandler::addKeyListener(jimp::KeyListener* keyListener) {
    keyListeners->push_back(keyListener);
}

}
