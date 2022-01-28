#ifndef bullet_hpp
#define bullet_hpp

#include "animatedSprite.hpp"
#include "direction.hpp"
#include "gameEngine.hpp"

class Bullet : public jimp::AnimatedSprite {
    
private:
    
public:
    static const int SPEED_IN_PIXELS_PER_SECOND;
    static const float SCALE;
    Bullet(float x, float y, float angle);
    void update(float elapsedTime);

};

#endif
