#ifndef asteroid_hpp
#define asteroid_hpp

#include "animatedSprite.hpp"
#include "gameEngine.hpp"
#include "bullet.hpp"
#include "sound.hpp"
#include "geo2D.hpp"

class Asteroid : public jimp::AnimatedSprite {
  
private:
    static const uint16_t FORCE;
    static const float HIT_ANIMATION_DURATION_IN_SECONDS;
    jimp::GameEngine* gameEngine = nullptr;
    jimp::Vector2D velocity = jimp::Vector2D { .x = 0, .y = 0 };
    float totalHitAnimationDuration = 0.0F;
    uint16_t rotationDegreesPerSecond = 0;
    bool rotatingDirection = false;
    bool isAnimatingHit = false;
    int hitCount = 0;
    bool isEnteringScreen = true;
    bool hasBeenFullyInsideScreen = false;
    jimp::Sound* hitSound = nullptr;
    void updateDirection(float elapsedTime);
    void updateMovement(float elapsedTime);
    void updateRotation(float elapsedTime);
    void animateHit(float elapsedTime);
    
public:
    Asteroid(jimp::GameEngine* gameEngine, float x, float y, float directionAngle);
    ~Asteroid();
    void doOnUpdate(float elapsedTime) override;
    void hasCollidedRectLeft(jimp::AnimatedSprite* otherSprite) override;
    void hasCollidedRectRight(jimp::AnimatedSprite* otherSprite) override;
    void hasCollidedRectTop(jimp::AnimatedSprite* otherSprite) override;
    void hasCollidedRectBottom(jimp::AnimatedSprite* otherSprite) override;
    void hasCollidedRect(jimp::AnimatedSprite* otherSprite, jimp::Geo2D::Side side) override;
    uint16_t getZIndex() override;
};

#endif
