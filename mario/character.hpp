#ifndef character_hpp
#define character_hpp

#include "graphic.hpp"
#include "vector2D.hpp"

class Character : public jimp::Graphic {
  
private:
    bool isMovingLeft = false;
    bool isMovingRight = false;
    
public:
    Character();
    void doOnUpdate(float elapsedTime) override;
    void onKeyboardLeft(jimp::KeyState keyState) override;
    void onKeyboardRight(jimp::KeyState keyState) override;
};

#endif
