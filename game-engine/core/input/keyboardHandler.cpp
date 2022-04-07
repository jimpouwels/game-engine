#include <list>
#include <SFML/Graphics.hpp>
#include "keyboardHandler.hpp"
#include "keyListener.hpp"
#include "scroller.hpp"
#include "gameEngine.hpp"

namespace jimp {

KeyboardHandler::KeyboardHandler() {
    keyListeners = new std::list<jimp::KeyListener*>();
    pressedKeys = new std::list<sf::Keyboard::Key>();
    events = new std::list<sf::Event>();
    mutex = new std::mutex();
}

KeyboardHandler::~KeyboardHandler() {
    delete keyListeners;
    delete pressedKeys;
}

void KeyboardHandler::addEvent(sf::Event event) {
    if (keyStateFor(event) == KeyState::PRESSED && pressedKeys->front() == event.key.code) {
        return;
    }
    mutex->lock();
    events->push_back(event);
    mutex->unlock();
}

void KeyboardHandler::handleAllEvents() {
    mutex->lock();
    for (const auto& event : *events) {
        KeyState keyState = keyStateFor(event);
        if (keyState == KeyState::PRESSED) {
            pressedKeys->push_front(event.key.code);
            handleKeyEvent(event.key.code, KeyState::PRESSED);
        } else {
            pressedKeys->remove(event.key.code);
            handleKeyEvent(event.key.code, KeyState::RELEASED);
            if (!pressedKeys->empty()) {
                handleKeyEvent(pressedKeys->front(), KeyState::PRESSED);
            }
        }
    }
    events->clear();
    mutex->unlock();
}

void KeyboardHandler::addKeyListener(jimp::KeyListener* keyListener) {
    keyListeners->push_back(keyListener);
}

void KeyboardHandler::removeKeyListener(jimp::KeyListener* keyListener) {
    keyListeners->remove(keyListener);
}

KeyState KeyboardHandler::keyStateFor(sf::Event event) {
    if (event.type == sf::Event::KeyPressed) {
        return PRESSED;
    } else {
        return RELEASED;
    }
}

void KeyboardHandler::handleKeyEvent(sf::Keyboard::Key key, KeyState keyState) {
    if (GameEngine::getInstance()->isEditMode() && key == sf::Keyboard::Space && keyState == KeyState::PRESSED) {
        GameEngine::getInstance()->reloadCurrentStage();
    }
    for (KeyListener* keyListener : *keyListeners) {
        if (GameEngine::getInstance()->isEditMode() && !(dynamic_cast<Scroller*>(keyListener))) {
            continue;
        }
        switch (key) {
            case sf::Keyboard::Left:
                keyListener->onKeyboardLeft(keyState);
                break;
            case sf::Keyboard::Right:
                keyListener->onKeyboardRight(keyState);
                break;
            case sf::Keyboard::Up:
                keyListener->onKeyboardUp(keyState);
                break;
            case sf::Keyboard::Down:
                keyListener->onKeyboardDown(keyState);
                break;
            case sf::Keyboard::Space:
                keyListener->onKeyboardSpaceBar(keyState);
                break;
        }
    }
}

}
