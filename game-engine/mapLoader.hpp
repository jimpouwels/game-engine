#ifndef mapLoader_hpp
#define mapLoader_hpp

#include "animatedGraphic.hpp"
#include <list>

namespace jimp {

class MapLoader {

public:
    std::list<AnimatedGraphic*>* loadGraphics();
    
};

}


#endif
