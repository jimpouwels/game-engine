#ifndef stageFactory_hpp
#define stageFactory_hpp

#include "animatedGraphic.hpp"
#include "dataLoader.hpp"
#include "type.hpp"
#include "animationType.hpp"

namespace jimp {

class StageFactory {
  
private:
    bool stageLoadInterrupted = false;
    bool threadManagerStopped = false;
    std::list<std::thread*>* loadThreads;
    std::thread* threadManager;
    std::list<Type*>* types = nullptr;
    DataLoader* dataLoader = nullptr;
    bool loadingSprites = false;
    void addSpritesToGraphic(AnimatedGraphic* animatedGraphic, AnimationType* animationType);
    void createAnimatedGraphicFrom(Graphic graphic);
    Type* getTypeFor(std::string typeName);
    
protected:
    virtual AnimatedGraphic* createCustomGraphic(std::string type) {
        return nullptr;
    };
    
public:
    StageFactory(std::string typesFilePath);
    ~StageFactory();
    void loadStage(std::string filePath);
    void manageThreads();
    bool isLoadingSprites();
    void stopProcessing();
    
};

}

#endif
