#ifndef fangoCharacter_hpp
#define fangoCharacter_hpp

#include <stdio.h>
#include <list>

#include "animatedSprite.hpp"
#include "keyListener.hpp"

namespace jimp {

class FangoCharacter : public jimp::AnimatedSprite, public jimp::KeyListener {
    
private:
    enum MoveDirection { MIN, PLUS, IDLE };
    static const int SPEED_IN_PIXELS_PER_SECOND;
    static const float IMAGE_SWAP_INTERVAL_IN_SECONDS;
    MoveDirection moveDirectionX = MoveDirection::IDLE;
    MoveDirection moveDirectionY = MoveDirection::IDLE;
    bool isMoving = false;
    void setMoving(KeyState keyState);
    int speedInPixelsPerSecond;
    
public:
    FangoCharacter();
    void update(float elapsedTime);
    void onKeyboardLeft(KeyState keyState) override;
    void onKeyboardRight(KeyState keyState) override;
    void onKeyboardUp(KeyState keyState) override;
    void onKeyboardDown(KeyState keyState) override;
};

}

#endif
