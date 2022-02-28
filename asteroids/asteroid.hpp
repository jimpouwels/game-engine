#ifndef asteroid_hpp
#define asteroid_hpp

#include "animatedGraphic.hpp"
#include "gameEngine.hpp"
#include "bullet.hpp"
#include "sound.hpp"
#include "geo2D.hpp"

class Asteroid : public jimp::AnimatedGraphic {
  
private:
    static const uint16_t SPEED_IN_PIXELS_PER_SECOND;
    static const float HIT_ANIMATION_DURATION_IN_SECONDS;
    jimp::Vector2D velocity = jimp::Vector2D { .x = 0, .y = 0 };
    float totalHitAnimationDuration = 0.0F;
    uint16_t rotationDegreesPerSecond = 0;
    bool rotatingDirection = false;
    bool isAnimatingHit = false;
    int hitCount = 0;
    float directionAngle = 0;
    bool isEnteringScreen = true;
    bool hasBeenFullyInsideScreen = false;
    jimp::Sound* hitSound = nullptr;
    void updateDirection(float elapsedTime);
    void updateMovement(float elapsedTime);
    void updateRotation(float elapsedTime);
    void animateHit(float elapsedTime);
    
public:
    Asteroid(jimp::Vector2D position, float directionAngle);
    ~Asteroid();
    void doOnInit() override;
    void doOnUpdate(float elapsedTime) override;
    void doOnFrame(float elapsedTime) override;
    void hasCollidedRectLeft(jimp::AnimatedGraphic* otherSprite) override;
    void hasCollidedRectRight(jimp::AnimatedGraphic* otherSprite) override;
    void hasCollidedRectTop(jimp::AnimatedGraphic* otherSprite) override;
    void hasCollidedRectBottom(jimp::AnimatedGraphic* otherSprite) override;
    void hasCollidedRect(jimp::AnimatedGraphic* otherSprite, jimp::Geo2D::Side side) override;
    uint16_t getZIndex() override;
};

#endif
