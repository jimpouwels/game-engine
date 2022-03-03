#ifndef floor_hpp
#define floor_hpp

#include "animatedGraphic.hpp"

namespace mario {
    
class Floor : public jimp::AnimatedGraphic {
  
private:
    uint16_t repeat = 0;
    
public:
    Floor(jimp::Vector2D position, uint16_t repeat);
    void doOnInit() override;
    void doOnUpdate(float elapsedTime) override;
};

}

#endif
