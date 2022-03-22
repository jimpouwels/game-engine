#ifndef movingPlatform_hpp
#define movingPlatform_hpp

#include "animatedGraphic.hpp"

namespace ghosty {

class MovingPlatform : public jimp::AnimatedGraphic {
  
private:
    float initialY = -1;
    
public:
    MovingPlatform();
    void doOnUpdate(float elapsedTime) override;
    
};

}

#endif
