#ifndef plant_hpp
#define plant_hpp

#include "decoration.hpp"
#include "animatedGraphic.hpp"

namespace ghosty {
    
class Decoration : public jimp::AnimatedGraphic {
  
private:
    uint16_t repeat = 0;
    int zIndex = 0;
    
public:
    Decoration(jimp::Vector2D position, std::string filePath, int imageCount, float rotationAngle, float scale, int zIndex);
    void doOnInit() override;
    void doOnUpdate(float elapsedTime) override;
    uint16_t getZIndex() override;
};

}

#endif
