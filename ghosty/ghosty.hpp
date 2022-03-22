#ifndef character_hpp
#define character_hpp

#include "animatedGraphic.hpp"
#include "vector2D.hpp"
#include "sound.hpp"

namespace ghosty {

class Ghosty : public jimp::AnimatedGraphic {
  
private:
    bool isJumping = false;
    jimp::Sound* footstepSound = nullptr;
    jimp::Sound* jumpSound = nullptr;
    
public:
    Ghosty();
    void doOnUpdate(float elapsedTime) override;
    void doOnFrame(float elapsedTime) override;
    void onKeyboardUp(jimp::KeyState keyState) override;
    void onKeyboardLeft(jimp::KeyState keyState) override;
    void onKeyboardRight(jimp::KeyState keyState) override;
    void hasCollidedRectLeft(AnimatedGraphic* otherGraphic) override;
    void hasCollidedRectRight(AnimatedGraphic* otherGraphic) override;
    void hasCollidedRectBottom(AnimatedGraphic* otherGraphic) override;
    void hasCollidedRectTop(AnimatedGraphic* otherGraphic) override;
    void hasNoCollisionWith(AnimatedGraphic* otherGraphic) override;
};

}

#endif
