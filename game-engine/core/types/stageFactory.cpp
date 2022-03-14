#include "stageFactory.hpp"

namespace jimp {

StageFactory::StageFactory(std::string typesFilePath) {
    dataLoader = new DataLoader(typesFilePath);
    this->types = dataLoader->loadTypes();
}

StageFactory::~StageFactory() {
    delete dataLoader;
    for (const auto& type : *types) {
        delete type;
    }
    delete types;
}

void StageFactory::loadStage(std::string filePath) {
    std::list<Graphic*> graphics = dataLoader->loadGraphics(filePath);
}

AnimatedGraphic* StageFactory::createGraphic(std::string type) {
    return nullptr;
}

}
