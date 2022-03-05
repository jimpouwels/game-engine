#ifndef backgroundRedCharacter_hpp
#define backgroundRedCharacter_hpp

#include "animatedGraphic.hpp"
#include "vector2D.hpp"

namespace mario {

class BackgroundRedCharacter : jimp::AnimatedGraphic {

private:
    float eyesClosedTime;
    float eyeOpenTime;
    
public:
    BackgroundRedCharacter(jimp::Vector2D position);
    void doOnInit() override;
    void doOnUpdate(float elapsedTime) override;
    uint16_t getZIndex() override;
};

}

#endif
