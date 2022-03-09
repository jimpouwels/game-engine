#ifndef floor_hpp
#define floor_hpp

#include "animatedGraphic.hpp"

namespace ghosty {
    
class PlatformBlock : public jimp::AnimatedGraphic {
  
private:
    
public:
    PlatformBlock(jimp::Vector2D position, std::string filePath, float angle, float scale);
    void doOnInit() override;
    void doOnUpdate(float elapsedTime) override;
    uint16_t getZIndex() override;
};

}

#endif
