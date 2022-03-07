#ifndef plant_hpp
#define plant_hpp

#include "animatedGraphic.hpp"

namespace mario {
    
class Plant : public jimp::AnimatedGraphic {
  
private:
    uint16_t repeat = 0;
    
public:
    Plant(jimp::Vector2D position, uint16_t repeat);
    void doOnInit() override;
    void doOnUpdate(float elapsedTime) override;
    uint16_t getZIndex() override;
};

}

#endif
