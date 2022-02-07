#ifndef keyListener_hpp
#define keyListener_hpp

#include <stdio.h>

namespace jimp {

enum KeyState { PRESSED, RELEASED };

class KeyListener {
    
public:
    virtual void onKeyboardLeft(KeyState keyState) { };
    virtual void onKeyboardRight(KeyState keyState) { };
    virtual void onKeyboardUp(KeyState keyState) { };
    virtual void onKeyboardDown(KeyState keyState) { };
    virtual void onKeyboardSpaceBar(KeyState keyState) { };
    
};

}

#endif
