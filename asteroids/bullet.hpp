#ifndef bullet_hpp
#define bullet_hpp

#include "graphic.hpp"

class Bullet : public jimp::Graphic {
    
private:
    static const uint16_t FORCE;
    
public:
    static const float SCALE;
    Bullet(jimp::Vector2D position, float angle);
    void doOnInit() override;
    void doOnUpdate(float elapsedTime) override;
    void doOnFrame(float elapsedTime) override;
    void hasCollidedRect(Graphic *otherSprite, jimp::Geo2D::Side side) override;
    uint16_t getZIndex() override;
};

#endif
