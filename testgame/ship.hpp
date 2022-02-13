#ifndef ship_hpp
#define ship_hpp

#include <stdio.h>
#include <list>

#include "animatedSprite.hpp"
#include "keyListener.hpp"
#include "shipEventListener.hpp"
#include "direction.hpp"
#include "vector2D.hpp"
#include "sound.hpp"

class Ship : public jimp::AnimatedSprite, public jimp::KeyListener {
    
private:
    static const float SCALE;
    static const uint16_t SPEED_IN_PIXELS_PER_SECOND;
    static const uint32_t THRUST_FORCE;
    static const uint16_t MASS;
    static const float IMAGE_SWAP_INTERVAL_IN_SECONDS;
    static const uint8_t ROTATION_DEGREES_PER_SECOND;
    static const uint8_t SHOTS_PER_SECOND;
    static const uint16_t ROTATION_POINT_Y_OFFSET; // the excaust flame makes the spaceship image non-symmetrical
    bool hasFired = false;
    bool isFiring = false;
    bool isThrothling = false;
    bool isRotatingLeft = false;
    bool isRotatingRight = false;
    int speedInPixelsPerSecond;
    jimp::Vector2D velocity = { .x = 0, .y = 0 };
    float elapsedTimeSinceLastShot = 0;
    ShipEventListener* eventListener = nullptr;
    jimp::Sound* firingSound = nullptr;
    jimp::Sound* thrustSound = nullptr;
    bool isThrustSoundFadingOut = false;
    void updateMovement(float elapsedTime);
    void updateFiring(float elapsedTime);
    void updateRotation(float elapsedTime);
    void updateSound(float elapsedTime);
    void setMoving(jimp::KeyState keyState);

public:
    Ship(jimp::GameEngine* gameEngine, ShipEventListener* shipEventListener);
    void onUpdate(float elapsedTime) override;
    void onKeyboardLeft(jimp::KeyState keyState) override;
    void onKeyboardRight(jimp::KeyState keyState) override;
    void onKeyboardUp(jimp::KeyState keyState) override;
    void onKeyboardSpaceBar(jimp::KeyState keyState) override;
    jimp::Vector2D getRotationPoint() override;
    void hasCollidedRect(jimp::AnimatedSprite* otherSprite, jimp::Geo2D::Side side) override;
};

#endif
