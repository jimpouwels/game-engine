#include <list>
#include <SFML/Graphics.hpp>
#include "keyboardHandler.hpp"
#include "keyListener.hpp"

namespace jimp {

KeyboardHandler::KeyboardHandler() {
    keyListeners = new std::list<jimp::KeyListener*>();
}

KeyboardHandler::~KeyboardHandler() {
    for (auto const& keyListener : *keyListeners) {
        delete keyListener;
    }
    delete keyListeners;
}

void KeyboardHandler::handleEvent(sf::Event event) {
    KeyState keyState = keyStateFor(event);
    switch (event.key.code) {
        case sf::Keyboard::Left:
            handleKeyboardLeft(keyState);
            break;
        case sf::Keyboard::Right:
            handleKeyboardRight(keyState);
            break;
        case sf::Keyboard::Up:
            handleKeyboardUp(keyState);
            break;
        case sf::Keyboard::Down:
            handleKeyboardDown(keyState);
            break;
    }
}

void KeyboardHandler::addKeyListener(jimp::KeyListener* keyListener) {
    keyListeners->push_back(keyListener);
}

void KeyboardHandler::handleKeyboardLeft(KeyState keyState) {
    for (auto const& keyListener : *keyListeners) {
        keyListener->onKeyboardLeft(keyState);
    }
}

void KeyboardHandler::handleKeyboardRight(KeyState keyState) {
    for (auto const& keyListener : *keyListeners) {
        keyListener->onKeyboardRight(keyState);
    }
}

void KeyboardHandler::handleKeyboardUp(KeyState keyState) {
    for (auto const& keyListener : *keyListeners) {
        keyListener->onKeyboardUp(keyState);
    }
}

void KeyboardHandler::handleKeyboardDown(KeyState keyState) {
    for (auto const& keyListener : *keyListeners) {
        keyListener->onKeyboardDown(keyState);
    }
}

KeyState KeyboardHandler::keyStateFor(sf::Event event) {
    if (event.type == sf::Event::KeyPressed) {
        return PRESSED;
    } else {
        return RELEASED;
    }
}

}
