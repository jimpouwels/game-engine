#ifndef fangoCharacter_hpp
#define fangoCharacter_hpp

#include <stdio.h>
#include <list>

#include "animatedSprite.hpp"
#include "keyListener.hpp"
#include "shipEventListener.hpp"
#include "direction.hpp"
#include "vector2D.hpp"

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
    jimp::Vector2D deltaVector = { .x = 0, .y = 0 };
    float elapsedTimeSinceLastShot = 0;
    ShipEventListener* eventListener = nullptr;
    void updateMovement(float elapsedTime);
    void updateFiring(float elapsedTime);
    void updateRotation(float elapsedTime);
    void setMoving(jimp::KeyState keyState);

public:
    Ship(jimp::GameEngine* gameEngine, ShipEventListener* shipEventListener);
    void onFrame(float elapsedTime) override;
    void onKeyboardLeft(jimp::KeyState keyState) override;
    void onKeyboardRight(jimp::KeyState keyState) override;
    void onKeyboardUp(jimp::KeyState keyState) override;
    void onKeyboardSpaceBar(jimp::KeyState keyState) override;
};

#endif
