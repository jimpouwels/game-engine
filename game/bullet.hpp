#ifndef bullet_hpp
#define bullet_hpp

#include "animatedSprite.hpp"

class Bullet : public jimp::AnimatedSprite {

private:

public:
    static const int SPEED_IN_PIXELS_PER_SECOND;
    static const float SCALE;
    Bullet(float x, float y);
    void update(float elapsedTime);

};

#endif
