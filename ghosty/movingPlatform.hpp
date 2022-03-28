#ifndef movingPlatform_hpp
#define movingPlatform_hpp

#include "animatedGraphic.hpp"

namespace ghosty {

class MovingPlatform : public jimp::AnimatedGraphic {
  
private:
    jimp::Vector2D startPosition = jimp::Vector2D::empty();
    jimp::Vector2D targetPosition = jimp::Vector2D::empty();
    
    
public:
    MovingPlatform(jimp::Vector2D startPosition, jimp::Vector2D targetPosition);
    void doOnFrame(float elapsedTime) override;
    void doOnUpdate(float elapsedTime) override;
    
};

}

#endif
