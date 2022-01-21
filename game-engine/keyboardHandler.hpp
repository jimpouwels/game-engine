#ifndef KeyboardHandler_hpp
#define KeyboardHandler_hpp

#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <list>
#include "keyListener.hpp"

namespace jimp {

class KeyboardHandler {
    
private:
    std::list<jimp::KeyListener*>* keyListeners;
    std::list<sf::Keyboard::Key>* pressedKeys;
    void handleKeyboardLeft(KeyState keyState);
    void handleKeyboardRight(KeyState keyState);
    void handleKeyboardUp(KeyState keyState);
    void handleKeyboardDown(KeyState keyState);
    void handleKeyboardSpaceBar(KeyState keyState);
    KeyState keyStateFor(sf::Event);
    void handleKeyEvent(sf::Keyboard::Key key, KeyState keyState);
    
public:
    KeyboardHandler();
    ~KeyboardHandler();
    void addKeyListener(jimp::KeyListener* keyListener);
    void handleEvent(sf::Event event);
    
};

}

#endif
