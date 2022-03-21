#ifndef raindrop_hpp
#define raindrop_hpp

#include "animatedGraphic.hpp"

namespace ghosty {

class Raindrop : public jimp::AnimatedGraphic {
    
public:
    Raindrop();
    ~Raindrop();
    void doOnUpdate(float elapsedTime) override;
};

}

#endif
