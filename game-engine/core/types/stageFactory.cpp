#include "stageFactory.hpp"
#include "animationType.hpp"
#include "gameEngine.hpp"
#include "simpleAnimation.hpp"

#include <string>
#include <regex>

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
    AnimatedGraphic* animatedGraphic = nullptr;
    Type* type = getTypeFor(graphic.type);
    if (dynamic_cast<AnimationType*>(type)) {
        AnimationType* animationType = dynamic_cast<AnimationType*>(type);
        if (animationType->custom) {
            animatedGraphic = createCustomGraphic(type->name);
        } else {
            animatedGraphic = new SimpleAnimation();
        }
        for (const auto& subAnimation : animationType->subAnimations) {
            for (int i = 0; i < subAnimation->spriteCount; i++) {
                animatedGraphic->addSprite(subAnimation->name, std::regex_replace(subAnimation->filePath, std::regex("\\{i\\}"), std::to_string(i)));
            }
        }
    }
    
    // set all stuff!! (sprites, rotation, gravity, etc)
    if (animatedGraphic != nullptr) {
        animatedGraphic->setPosition(graphic.position);
        animatedGraphic->setScale(graphic.scale);
        animatedGraphic->setRotationAngle(graphic.rotationAngle);
        animatedGraphic->setApplyScrolling(graphic.applyScrolling);
        animatedGraphic->setApplyGravity(graphic.applyGravity);
    
        animatedGraphic->setSpriteSwapInterval(type->spriteSwapInterval);
        
        GameEngine::getInstance()->registerGraphic(animatedGraphic);
    }
    return animatedGraphic;
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
