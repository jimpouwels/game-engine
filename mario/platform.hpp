#ifndef platform_hpp
#define platform_hpp

#include "animatedGraphic.hpp"
#include "vector2D.hpp"

namespace mario {

class Platform : public jimp::AnimatedGraphic {
  
private:
    uint16_t repeat = 0;
    bool hit = false;
    
public:
    Platform(jimp::Vector2D position, uint16_t repeat);
    void doOnInit() override;
    void doOnUpdate(float elapsedTime) override;
    void hasCollidedRectTop(AnimatedGraphic* otherGraphic) override;
};

}

#endif
