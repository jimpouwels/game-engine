#ifndef bullet_hpp
#define bullet_hpp

#include "animatedSprite.hpp"

class Bullet : public jimp::AnimatedSprite {

private:

public:
    Bullet(float x, float y);
    void update(float elapsedTime);

};

#endif
