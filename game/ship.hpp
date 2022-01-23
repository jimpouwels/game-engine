#ifndef fangoCharacter_hpp
#define fangoCharacter_hpp

#include <stdio.h>
#include <list>

#include "animatedSprite.hpp"
#include "keyListener.hpp"
#include "fangoEventListener.hpp"
#include "direction.hpp"
#include "gameEngine.hpp"

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
    float elapsedTimeSinceLastShot = 0;
    FangoEventListener* eventListener = nullptr;
    void handleMovement(float elapsedTime);
    void handleFiring(float elapsedTime);
    void setMoving(jimp::KeyState keyState);

public:
    Ship(jimp::GameEngine* gameEngine, FangoEventListener* fangoEventListener);
    void update(float elapsedTime);
    void onKeyboardLeft(jimp::KeyState keyState) override;
    void onKeyboardRight(jimp::KeyState keyState) override;
    void onKeyboardUp(jimp::KeyState keyState) override;
    void onKeyboardDown(jimp::KeyState keyState) override;
    void onKeyboardSpaceBar(jimp::KeyState keyState) override;
};

#endif
