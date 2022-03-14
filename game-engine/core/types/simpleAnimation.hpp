#ifndef plant_hpp
#define plant_hpp

#include "simpleAnimation.hpp"
#include "animatedGraphic.hpp"

namespace jimp {
    
class SimpleAnimation : public jimp::AnimatedGraphic {
  
private:
    uint16_t repeat = 0;
    int zIndex = 0;
    
public:
    SimpleAnimation();
    void doOnUpdate(float elapsedTime) override;
};

}

#endif
