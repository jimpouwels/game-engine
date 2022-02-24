#ifndef ship_hpp
#define ship_hpp

#include <stdio.h>
#include <list>

#include "graphic.hpp"
#include "vector2D.hpp"
#include "sound.hpp"

class Ship : public jimp::Graphic {
    
private:
    static const float SCALE;
    static const uint16_t SPEED_IN_PIXELS_PER_SECOND;
    static const uint32_t THRUST_FORCE;
    static const uint16_t MASS;
    static const float IMAGE_SWAP_INTERVAL_IN_SECONDS;
    static const uint8_t ROTATION_DEGREES_PER_SECOND;
    static const uint8_t SHOTS_PER_SECOND;
    static const uint16_t ROTATION_POINT_Y_OFFSET; // the exhaust flame makes the spaceship image non-symmetrical
    bool hasFired = false;
    bool isFiring = false;
    bool isThrothling = false;
    bool isRotatingLeft = false;
    bool isRotatingRight = false;
    int speedInPixelsPerSecond;
    float elapsedTimeSinceLastShot = 0;
    jimp::Sound* firingSound = nullptr;
    jimp::Sound* thrustSound = nullptr;
    bool isThrustSoundFadingOut = false;
    void updateMovement(float elapsedTime);
    void updateFiring(float elapsedTime);
    void updateRotation(float elapsedTime);
    void setMoving(jimp::KeyState keyState);

public:
    Ship();
    void doOnInit() override;
    void doOnUpdate(float elapsedTime) override;
    void doOnFrame(float elapsedTime) override;
    void onKeyboardLeft(jimp::KeyState keyState) override;
    void onKeyboardRight(jimp::KeyState keyState) override;
    void onKeyboardUp(jimp::KeyState keyState) override;
    void onKeyboardSpaceBar(jimp::KeyState keyState) override;
    jimp::Vector2D getRotationPoint() override;
    void hasCollidedRect(jimp::Graphic* otherSprite, jimp::Geo2D::Side side) override;
    void hasCollidedRectRight(jimp::Graphic* otherSprite) override;
    void hasCollidedRectLeft(jimp::Graphic* otherSprite) override;
    void hasCollidedRectTop(jimp::Graphic* otherSprite) override;
    void hasCollidedRectBottom(jimp::Graphic* otherSprite) override;
    uint16_t getZIndex() override;
};

#endif
