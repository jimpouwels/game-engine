#ifndef asteroid_hpp
#define asteroid_hpp

#include "animatedSprite.hpp"
#include "gameEngine.hpp"
#include "bullet.hpp"

class Asteroid : public jimp::AnimatedSprite {
  
private:
    static const int SPEED_IN_PIXELS_PER_SECOND;
    static const int ROTATION_DEGREES_PER_SECOND;
    static const float HIT_ANIMATION_DURATION_IN_SECONDS;
    float directionAngle;
    float totalHitAnimationDuration = 0.0F;
    bool isHit = false;
    bool destroyed = false;
    int hitCount = 0;
    bool isEnteringScreen = true;
    bool hasBeenFullyInsideScreen = false;
    void updateDirection(float elapsedTime);
    void updateMovement(float elapsedTime);
    void updateRotation(float elapsedTime);
    void handleHits(float elapsedTime);
    
public:
    Asteroid(jimp::GameEngine* gameEngine, float x, float y, float directionAngle);
    ~Asteroid();
    void onFrame(float elapsedTime) override;
    void setHit();
    bool isHitBy(jimp::AnimatedSprite& animatedSprite);
    bool isDestroyed();
};

#endif
