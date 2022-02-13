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
    static const uint16_t ROTATION_DEGREES_PER_SECOND;
    static const float HIT_ANIMATION_DURATION_IN_SECONDS;
    float directionAngle;
    float totalHitAnimationDuration = 0.0F;
    bool isHit = false;
    int hitCount = 0;
    bool isEnteringScreen = true;
    bool hasBeenFullyInsideScreen = false;
    jimp::Sound* hitSound = nullptr;
    void updateDirection(float elapsedTime);
    void updateMovement(float elapsedTime);
    void updateRotation(float elapsedTime);
    void handleHits(float elapsedTime);
    
public:
    Asteroid(jimp::GameEngine* gameEngine, float x, float y, float directionAngle);
    ~Asteroid();
    void onUpdate(float elapsedTime) override;
    void hasCollidedRectLeft(jimp::AnimatedSprite* otherSprite) override;
    void hasCollidedRectRight(jimp::AnimatedSprite* otherSprite) override;
    void hasCollidedRectTop(jimp::AnimatedSprite* otherSprite) override;
    void hasCollidedRectBottom(jimp::AnimatedSprite* otherSprite) override;
    void hasCollidedRect(jimp::AnimatedSprite* otherSprite, jimp::Geo2D::Side side) override;
};

#endif
