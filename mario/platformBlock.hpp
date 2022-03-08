#ifndef floor_hpp
#define floor_hpp

#include "animatedGraphic.hpp"

namespace ghosty {
    
class PlatformBlock : public jimp::AnimatedGraphic {
  
private:
    uint16_t repeat = 0;
    
public:
    PlatformBlock(jimp::Vector2D position, std::string filePath, uint16_t repeat);
    void doOnInit() override;
    void doOnUpdate(float elapsedTime) override;
};

}

#endif
