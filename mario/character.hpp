#ifndef character_hpp
#define character_hpp

#include "animatedGraphic.hpp"
#include "vector2D.hpp"

class Character : public jimp::AnimatedGraphic {
  
private:
    bool isMovingLeft = false;
    bool isMovingRight = false;
    
public:
    Character();
    void doOnInit() override;
    void doOnUpdate(float elapsedTime) override;
    void onKeyboardUp(jimp::KeyState keyState) override;
    void onKeyboardLeft(jimp::KeyState keyState) override;
    void onKeyboardRight(jimp::KeyState keyState) override;
};

#endif
