#ifndef floor_hpp
#define floor_hpp

#include "animatedGraphic.hpp"

namespace mario {
    
class Floor : public jimp::AnimatedGraphic {
  
private:
    uint16_t repeat = 0;
    
public:
    Floor(jimp::Vector2D position, std::string filePath, uint16_t repeat);
    void doOnInit() override;
    void doOnUpdate(float elapsedTime) override;
};

}

#endif
