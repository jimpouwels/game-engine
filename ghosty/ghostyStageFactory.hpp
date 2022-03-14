#ifndef ghostyGraphicFactory_hpp
#define ghostyGraphicFactory_hpp

#include "ghostyStageFactory.hpp"
#include "stageFactory.hpp"


namespace ghosty {

class GhostyStageFactory : public jimp::StageFactory {
  
protected:
    jimp::AnimatedGraphic* createCustomGraphic(std::string name) override;
    
public:
    GhostyStageFactory(std::string typesFilePath);
    
};

}

#endif
