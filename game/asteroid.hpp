#ifndef asteroid_hpp
#define asteroid_hpp

#include "animatedSprite.hpp"
#include "screen.hpp"

class Asteroid : public jimp::AnimatedSprite {
  
private:
    enum DirectionX { RIGHT, LEFT };
    enum DirectionY { UP, DOWN };
    static const int SPEED_IN_PIXELS_PER_SECOND;
    static const int ROTATION_DEGREES_PER_SECOND;
    DirectionX directionX;
    DirectionY directionY;
    
public:
    Asteroid(jimp::Screen* screen, float x, float y);
    ~Asteroid();
    void update(float elapsedTime);
};

#endif
