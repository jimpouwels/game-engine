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
    SimpleAnimation(jimp::Vector2D position, std::string filePath, int imageCount, float rotationAngle, float scale, int zIndex);
    void doOnInit() override;
    void doOnUpdate(float elapsedTime) override;
    uint16_t getZIndex() override;
};

}

#endif
