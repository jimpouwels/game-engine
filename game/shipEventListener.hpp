#ifndef shipEventListener_h
#define shipEventListener_h

#include "bullet.hpp"

class ShipEventListener {
    
public:
    virtual void onWeaponFired(jimp::AnimatedSprite* projectile) = 0;
    
};

#endif
