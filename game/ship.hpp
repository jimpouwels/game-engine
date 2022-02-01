#ifndef fangoCharacter_hpp
#define fangoCharacter_hpp

#include <stdio.h>
#include <list>

#include "animatedSprite.hpp"
#include "keyListener.hpp"
#include "shipEventListener.hpp"
#include "direction.hpp"
#include "screen.hpp"
#include "position2D.hpp"

class Ship : public jimp::AnimatedSprite, public jimp::KeyListener {
    
private:
    static const float SCALE;
    static const int SPEED_IN_PIXELS_PER_SECOND;
    static const float IMAGE_SWAP_INTERVAL_IN_SECONDS;
    static const int ROTATION_DEGREES_PER_SECOND;
    static const int SHOTS_PER_SECOND;
    bool hasFired = false;
    bool isFiring = false;
    bool isThrothling = false;
    bool isRotatingLeft = false;
    bool isRotatingRight = false;
    int speedInPixelsPerSecond;
    jimp::Position2D deltaPosition = { .x = 0, .y = 0 };
    float elapsedTimeSinceLastShot = 0;
    ShipEventListener* eventListener = nullptr;
    void handleMovement(float elapsedTime);
    void handleFiring(float elapsedTime);
    void handleRotation(float elapsedTime);
    void setMoving(jimp::KeyState keyState);

public:
    Ship(jimp::Screen* screen, ShipEventListener* shipEventListener);
    void update(float elapsedTime) override;
    void onKeyboardLeft(jimp::KeyState keyState) override;
    void onKeyboardRight(jimp::KeyState keyState) override;
    void onKeyboardUp(jimp::KeyState keyState) override;
    void onKeyboardDown(jimp::KeyState keyState) override;
    void onKeyboardSpaceBar(jimp::KeyState keyState) override;
};

#endif
