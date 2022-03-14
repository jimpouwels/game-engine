#include "stageFactory.hpp"
#include "animationType.hpp"

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
    std::list<Graphic> graphics = dataLoader->loadGraphics(filePath);
    for (const auto& graphic : graphics) {
        createAnimatedGraphicFrom(graphic);
    }
}

AnimatedGraphic* StageFactory::createAnimatedGraphicFrom(Graphic graphic) {
    Type* type = getTypeFor(graphic.type);
    if (dynamic_cast<AnimationType*>(type)) {
        AnimationType* animationType = dynamic_cast<AnimationType*>(type);
        if (animationType->custom) {
            createCustomGraphic(type->name);
        } else {
            // TODO create default graphic type
        }
    }
    return nullptr;
}

Type* StageFactory::getTypeFor(std::string typeName) {
    for (const auto& type : *types) {
        if (type->name == typeName) {
            return type;
        }
    }
    return nullptr;
}

}
