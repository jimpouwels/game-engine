#ifndef fangoEventListener_h
#define fangoEventListener_h

#include "bullet.hpp"

class FangoEventListener {
    
public:
    virtual void onWeaponFired(jimp::AnimatedSprite* projectile) = 0;
    
};

#endif
