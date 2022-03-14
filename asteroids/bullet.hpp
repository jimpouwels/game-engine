#ifndef bullet_hpp
#define bullet_hpp

#include "animatedGraphic.hpp"

class Bullet : public jimp::AnimatedGraphic {
    
private:
    static const uint16_t FORCE;
    
public:
    static const float SCALE;
    Bullet(jimp::Vector2D position, float angle);
    void doOnUpdate(float elapsedTime) override;
    void doOnFrame(float elapsedTime) override;
    void hasCollidedRect(AnimatedGraphic *otherSprite, jimp::Geo2D::Side side) override;
};

#endif
