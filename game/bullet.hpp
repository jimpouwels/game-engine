#ifndef bullet_hpp
#define bullet_hpp

#include "animatedSprite.hpp"
#include "direction.hpp"

class Bullet : public jimp::AnimatedSprite {
    
private:
    Direction direction;
    
public:
    static const int SPEED_IN_PIXELS_PER_SECOND;
    static const float SCALE;
    Bullet(float x, float y, Direction direction);
    void update(float elapsedTime);

};

#endif
