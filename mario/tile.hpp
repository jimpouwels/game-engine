#ifndef tile_hpp
#define tile_hpp

#include "animatedGraphic.hpp"

namespace mario {

class Tile {
  
private:
    jimp::AnimatedGraphic* animatedGraphic = nullptr;
    
public:
    jimp::AnimatedGraphic* getAnimatedGraphic();
    void add(jimp::AnimatedGraphic* animatedGraphic);

};

}

#endif
