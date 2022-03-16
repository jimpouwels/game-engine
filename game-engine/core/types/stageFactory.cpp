#include "stageFactory.hpp"
#include "animationType.hpp"
#include "platformMultiLayerType.hpp"
#include "platformSingleLayerType.hpp"
#include "gameEngine.hpp"
#include "platformMultiBuilder.hpp"
#include "platformSingleBuilder.hpp"

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

void StageFactory::createAnimatedGraphicFrom(Graphic graphic) {
    Type* type = getTypeFor(graphic.type);
    if (dynamic_cast<AnimationType*>(type)) {
        AnimationType* animationType = dynamic_cast<AnimationType*>(type);
        AnimatedGraphic* animatedGraphic = nullptr;
        if (animationType->custom) {
            animatedGraphic = createCustomGraphic(type->name);
        } else {
            animatedGraphic = new AnimatedGraphic();
        }
        for (const auto& subAnimation : animationType->subAnimations) {
            for (int i = 0; i < subAnimation->spriteCount; i++) {
                animatedGraphic->addSprite(subAnimation->name, std::regex_replace(subAnimation->filePath, std::regex("\\{i\\}"), std::to_string(i)));
            }
        }
        animatedGraphic->setPosition(graphic.position);
        animatedGraphic->setScale(graphic.scale);
        animatedGraphic->setRotationAngle(graphic.rotationAngle);
        animatedGraphic->setApplyScrolling(graphic.applyScrolling);
        animatedGraphic->setApplyGravity(graphic.applyGravity);
        animatedGraphic->setSpriteSwapInterval(type->spriteSwapInterval);
        animatedGraphic->setZIndex(graphic.zIndex);
        animatedGraphic->setCollidable(graphic.collidable);
        if (graphic.isMainCharacter) {
            ScrollingWorld::getInstance()->setMainCharacter(animatedGraphic);
            animatedGraphic->setApplyScrolling(false);
        }
        GameEngine::getInstance()->registerGraphic(animatedGraphic);
    } else if (dynamic_cast<PlatformMultiLayerType*>(type)) {
        PlatformMultiLayerType* animationType = dynamic_cast<PlatformMultiLayerType*>(type);
        PlatformMultiBuilder* builder = new PlatformMultiBuilder(graphic.rows, graphic.cols, animationType->spriteSize, graphic.scale, graphic.position);
        builder->setCornerTopLeft(animationType->cornerLeftTopFilePath);
        builder->setCornerTopRight(animationType->cornerRightTopFilePath);
        builder->setCornerBottomLeft(animationType->cornerLeftBottomFilePath);
        builder->setCornerBottomRight(animationType->cornerRightBottomFilePath);
        builder->setMiddleTop(animationType->middleTopFilePath);
        builder->setBottomMiddle(animationType->middleBottomFilePath);
        builder->setRightMiddle(animationType->middleRightFilePath);
        builder->setLeftMiddle(animationType->middleLeftFilePath);
        builder->setCenter(animationType->centerFilePath);
        builder->setZIndex(graphic.zIndex);
        builder->render();
    } else if (dynamic_cast<PlatformSingleLayerType*>(type)) {
        PlatformSingleLayerType* animationType = dynamic_cast<PlatformSingleLayerType*>(type);
        PlatformSingleBuilder* builder = new PlatformSingleBuilder(graphic.cols, animationType->spriteSize, graphic.scale, graphic.position);
        builder->setCornerLeft(animationType->cornerLeftFilePath);
        builder->setCornerRight(animationType->cornerRightFilePath);
        builder->setMiddle(animationType->centerFilePath);
        builder->setZIndex(graphic.zIndex);
        builder->render();
    }
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
