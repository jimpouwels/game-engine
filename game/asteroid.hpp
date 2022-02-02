#ifndef asteroid_hpp
#define asteroid_hpp

#include "animatedSprite.hpp"
#include "screen.hpp"

class Asteroid : public jimp::AnimatedSprite {
  
private:
    static const int SPEED_IN_PIXELS_PER_SECOND;
    static const int ROTATION_DEGREES_PER_SECOND;
    float directionAngle;
    void updateDirection(float elapsedTime);
    void updateMovement(float elapsedTime);
    void updateRotation(float elapsedTime);
    
public:
    Asteroid(jimp::Screen* screen, float x, float y);
    ~Asteroid();
    void update(float elapsedTime);
};

#endif
