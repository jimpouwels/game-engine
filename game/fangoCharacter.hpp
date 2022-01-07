#ifndef fangoCharacter_hpp
#define fangoCharacter_hpp

#include <stdio.h>

#include "animatedCharacter.hpp"
#include "keyListener.hpp"

namespace jimp {

class FangoCharacter : public jimp::AnimatedCharacter, public jimp::KeyListener {
    
public:
    FangoCharacter();
    void onKeyboardLeft(KeyState keyState) override;
    void onKeyboardRight(KeyState keyState) override;
    void onKeyboardUp(KeyState keyState) override;
    void onKeyboardDown(KeyState keyState) override;
};

}

#endif
