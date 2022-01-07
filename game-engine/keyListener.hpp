#ifndef keyListener_hpp
#define keyListener_hpp

#include <stdio.h>

namespace jimp {

class KeyListener {
    
public:
    virtual void onLeftButtonPressed() { };
    virtual void onRightButtonPressed() { };
    virtual void onUpButtonPressed() { };
    virtual void onBottomButtonPressed() { };
    
};

}

#endif
