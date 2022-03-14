#ifndef dataLoader_hpp
#define dataLoader_hpp

#include "animatedGraphic.hpp"
#include "type.hpp"
#include "graphic.hpp"
#include <list>

namespace jimp {

class DataLoader {

private:
    std::string typesFilePath;
    
public:
    DataLoader(std::string typesFilePath);
    std::list<Type*>* loadTypes();
    std::list<Graphic*>* loadGraphics(std::string filePath);
    
};

}


#endif
