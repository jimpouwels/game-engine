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
    void handleKeyboardLeft(KeyState keyState);
    void handleKeyboardRight(KeyState keyState);
    void handleKeyboardUp(KeyState keyState);
    void handleKeyboardDown(KeyState keyState);
    KeyState keyStateFor(sf::Event);
    
public:
    KeyboardHandler();
    ~KeyboardHandler();
    void addKeyListener(jimp::KeyListener* keyListener);
    void handleEvent(sf::Event event);
    
};

}

#endif
