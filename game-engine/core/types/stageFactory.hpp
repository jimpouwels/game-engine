#ifndef graphicFactory_hpp
#define graphicFactory_hpp

#include "animatedGraphic.hpp"
#include "dataLoader.hpp"
#include "type.hpp"

namespace jimp {

class StageFactory {
  
private:
    std::list<Type*>* types = nullptr;
    DataLoader* dataLoader = nullptr;
    AnimatedGraphic* createAnimatedGraphicFrom(Graphic graphic);
    Type* getTypeFor(std::string typeName);
    
protected:
    virtual AnimatedGraphic* createCustomGraphic(std::string type) = 0;
    
public:
    void loadStage(std::string filePath);
    StageFactory(std::string typesFilePath);
    ~StageFactory();
    
};

}

#endif
