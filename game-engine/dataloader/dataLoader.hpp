#ifndef dataLoader_hpp
#define dataLoader_hpp

#include "json.hpp"
#include "animatedGraphic.hpp"
#include "type.hpp"
#include "graphic.hpp"
#include <list>

namespace jimp {

class DataLoader {

private:
    std::string typesFilePath;
    std::string loadFileContents(std::string filePath);
    Type* createType(nlohmann::json typeJson);
    
public:
    DataLoader(std::string typesFilePath);
    std::list<Type*>* loadTypes();
    std::list<Graphic*>* loadGraphics(std::string filePath);
};

}


#endif
