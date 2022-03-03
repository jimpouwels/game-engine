#ifndef platform_hpp
#define platform_hpp

#include "animatedGraphic.hpp"
#include "vector2D.hpp"

namespace mario {

class Platform : public jimp::AnimatedGraphic {
  
private:
    
public:
    Platform(jimp::Vector2D position);
    void doOnInit() override;
    void doOnUpdate(float elapsedTime) override;
};

}

#endif
