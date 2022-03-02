#ifndef character_hpp
#define character_hpp

#include "animatedGraphic.hpp"
#include "vector2D.hpp"

namespace mario {

class Character : public jimp::AnimatedGraphic {
  
private:
    bool hasCollidedWithBlocker = false;
    
public:
    Character();
    void doOnInit() override;
    void doOnUpdate(float elapsedTime) override;
    void onKeyboardUp(jimp::KeyState keyState) override;
    void onKeyboardLeft(jimp::KeyState keyState) override;
    void onKeyboardRight(jimp::KeyState keyState) override;
    void hasCollidedRectLeft(AnimatedGraphic* otherGraphic) override;
    void hasCollidedRectRight(AnimatedGraphic* otherGraphic) override;
    void hasCollidedRectBottom(AnimatedGraphic* otherGraphic) override;
    void hasCollidedRectTop(AnimatedGraphic* otherGraphic) override;
};

}

#endif
