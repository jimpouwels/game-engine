#ifndef gamingInterface_hpp
#define gamingInterface_hpp

#include <map>
#include "screen.hpp"
#include "image.hpp"

namespace jimp {

class GamingInterface {
    
private:
    Screen* screen = nullptr;
    std::map<std::string, Image*>* imageCache = nullptr;
    
public:
    GamingInterface(Screen* screen);
    Screen* getScreen();
    Image* loadImage(std::string filePath);
    
};

}

#endif
