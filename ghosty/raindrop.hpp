#ifndef raindrop_hpp
#define raindrop_hpp

#include "animatedGraphic.hpp"

namespace ghosty {

class Raindrop : public jimp::AnimatedGraphic {
    
private:
    int speedY = 0;
    int speedX = 0;
    int initialX = 0;
    
public:
    Raindrop();
    ~Raindrop();
    void doOnUpdate(float elapsedTime) override;
};

}

#endif
