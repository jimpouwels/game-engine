#ifndef fangoCharacter_hpp
#define fangoCharacter_hpp

#include <stdio.h>
#include <list>

#include "animatedCharacter.hpp"
#include "keyListener.hpp"

namespace jimp {

class FangoCharacter : public jimp::AnimatedCharacter, public jimp::KeyListener {
    
private:
    static const int SPEED_IN_PIXELS_PER_SECOND;
    static const float IMAGE_SWAP_INTERVAL_IN_SECONDS;
    
    enum MoveDirection { NORTH, SOUTH, WEST, EAST } moveDirection;
    bool isMoving;
    void setMoving(KeyState keyState, MoveDirection moveDirection);
    int speedInPixelsPerSecond;
    
public:
    FangoCharacter();
    void updatePosition(float elapsedTime);
    void onKeyboardLeft(KeyState keyState) override;
    void onKeyboardRight(KeyState keyState) override;
    void onKeyboardUp(KeyState keyState) override;
    void onKeyboardDown(KeyState keyState) override;
};

}

#endif
