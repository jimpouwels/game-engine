#ifndef asteroid_hpp
#define asteroid_hpp

#include "animatedSprite.hpp"

class Asteroid : public jimp::AnimatedSprite {
  
private:
    static const int SPEED_IN_PIXELS_PER_SECOND;
    static const int ROTATION_DEGREES_PER_SECOND;
    
public:
    Asteroid(float x, float y);
    ~Asteroid();
    void update(float elapsedTime);
};

#endif
