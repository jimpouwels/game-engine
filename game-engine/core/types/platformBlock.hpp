#ifndef floor_hpp
#define floor_hpp

#include "animatedGraphic.hpp"

namespace jimp {
    
class PlatformBlock : public jimp::AnimatedGraphic {
  
private:
    
public:
    PlatformBlock(jimp::Vector2D position, std::string filePath, float angle, float scale, int zIndex);
    void doOnUpdate(float elapsedTime) override;
    void doOnFrame(float elapsedTime) override;
};

}

#endif
