#ifndef character_hpp
#define character_hpp

#include "vector2D.hpp"

class Character {
  
private:
    jimp::Vector2D position = jimp::Vector2D { .x = 0, .y = 0 };
    
public:
    Character();
    void onUpdate(float elapsedTime);
    void onFrame(float elapsedTime);
    
};

#endif
