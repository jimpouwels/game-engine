#ifndef bullet_hpp
#define bullet_hpp

#include "animatedSprite.hpp"
#include "direction.hpp"
#include "gameEngine.hpp"

class Bullet : public jimp::AnimatedSprite {
    
private:
    
public:
    static const uint16_t FORCE;
    static const float SCALE;
    Bullet(jimp::GameEngine* gameEngine, float x, float y, float angle);
    void onUpdate(float elapsedTime) override;
    bool isPositionedWithinScreen() override;
    void hasCollidedRect(AnimatedSprite *otherSprite, jimp::Geo2D::Side side) override;
};

#endif
