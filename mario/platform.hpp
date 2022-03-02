#ifndef platform_hpp
#define platform_hpp

#include "animatedGraphic.hpp"
#include "vector2D.hpp"

namespace mario {

class Platform : public jimp::AnimatedGraphic {
  
private:
    int width = 0;
    
public:
    Platform(jimp::Vector2D position, int width);
    void doOnInit() override;
    void doOnUpdate(float elapsedTime) override;
};

}

#endif
