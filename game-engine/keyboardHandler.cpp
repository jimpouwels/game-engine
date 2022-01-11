#include <list>
#include <SFML/Graphics.hpp>
#include "keyboardHandler.hpp"
#include "keyListener.hpp"

namespace jimp {

KeyboardHandler::KeyboardHandler() {
    keyListeners = new std::list<jimp::KeyListener*>();
    pressedKeys = new std::list<sf::Keyboard::Key>();
}

KeyboardHandler::~KeyboardHandler() {
    for (auto const& keyListener : *keyListeners) {
        delete keyListener;
    }
    delete keyListeners;
    delete pressedKeys;
}

void KeyboardHandler::handleEvent(sf::Event event) {
    KeyState keyState = keyStateFor(event);
    
    if (keyState == KeyState::PRESSED) {
        if (pressedKeys->front() == event.key.code) {
            return;
        }
        pressedKeys->push_front(event.key.code);
        handleKeyEvent(event.key.code, KeyState::PRESSED);
    } else {
        pressedKeys->remove(event.key.code);
        handleKeyEvent(event.key.code, KeyState::RELEASED);
        if (pressedKeys->size() > 0) {
            handleKeyEvent(pressedKeys->front(), KeyState::PRESSED);
        }
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

void KeyboardHandler::handleKeyEvent(sf::Keyboard::Key key, KeyState keyState) {
    switch (key) {
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

}
