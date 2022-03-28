#include "stageFactory.hpp"
#include "animationType.hpp"
#include "platformMultiLayerType.hpp"
#include "platformSingleLayerType.hpp"
#include "gameEngine.hpp"
#include "platformMultiFactory.hpp"
#include "platformSingleFactory.hpp"

#include <string>
#include <regex>

namespace jimp {

StageFactory::StageFactory(std::string typesFilePath) {
    dataLoader = new DataLoader(typesFilePath);
    this->types = dataLoader->loadTypes();
    this->spriteLoaderThreads = new std::list<std::thread*>();
    this->threadManager = new std::thread(&StageFactory::manageSpriteLoaderThreads, this);
}

StageFactory::~StageFactory() {
    stageLoadStopped = true;
    stopCurrentLoadingAction();
    while (!threadManagerStopped) {
    }
    threadManager->join();
    delete threadManager;
    delete dataLoader;
    for (const auto& type : *types) {
        delete type;
    }
    delete types;
    delete spriteLoaderThreads;
}

void StageFactory::stopCurrentLoadingAction() {
    stageLoadInterrupted = true;
    while (spriteLoaderThreads->size() > 0) {
    }
    stageLoadInterrupted = false;
}

void StageFactory::loadStage(std::string filePath) {
    stopCurrentLoadingAction();
    std::list<Graphic> graphics = dataLoader->loadGraphics(filePath);
    for (const auto& graphic : graphics) {
        if (stageLoadInterrupted) {
            return;
        }
        createAnimatedGraphicFrom(graphic);
    }
}

void StageFactory::createAnimatedGraphicFrom(Graphic graphic) {
    Type* type = getTypeFor(graphic.type);
    if (dynamic_cast<AnimationType*>(type)) {
        AnimationType* animationType = dynamic_cast<AnimationType*>(type);
        AnimatedGraphic* animatedGraphic = nullptr;
        if (animationType->custom) {
            animatedGraphic = createCustomGraphic(type->name, graphic);
        } else {
            animatedGraphic = new AnimatedGraphic();
        }
        animatedGraphic->setPosition(graphic.position);
        animatedGraphic->setScale(graphic.scale);
        animatedGraphic->setRotationAngle(graphic.rotationAngle);
        animatedGraphic->setApplyGravity(graphic.applyGravity);
        animatedGraphic->setZIndex(graphic.zIndex);
        animatedGraphic->setCollidable(graphic.collidable);
        animatedGraphic->setName(graphic.description);
        animatedGraphic->setTransparency(graphic.transparency);
        if (graphic.animateRgb) {
            animatedGraphic->setRgbLevels(graphic.animateRgbFrom);
            animatedGraphic->animateRgbLevels(graphic.animateRgbTo, graphic.animateRgbSeconds);
        } else {
            animatedGraphic->setRgbLevels(graphic.rgb);
        }
        animatedGraphic->setApplyScrolling(graphic.applyScrolling || (graphic.isMainCharacter && GameEngine::getInstance()->isEditMode()));
        if (graphic.isMainCharacter) {
            ScrollingWorld::getInstance()->setMainCharacter(animatedGraphic);
        }
        spriteLoaderThreads->push_back(new std::thread(&StageFactory::addSpritesToGraphic, this, animatedGraphic, animationType));
        GameEngine::getInstance()->registerGraphic(animatedGraphic);
    } else if (dynamic_cast<PlatformMultiLayerType*>(type)) {
        PlatformMultiLayerType* animationType = dynamic_cast<PlatformMultiLayerType*>(type);
        PlatformMultiFactory builder = PlatformMultiFactory(graphic.rows, graphic.cols, animationType->spriteSize, graphic.scale, graphic.transparency, graphic.position);
        builder.setCornerTopLeft(animationType->cornerLeftTopFilePath);
        builder.setCornerTopRight(animationType->cornerRightTopFilePath);
        builder.setCornerBottomLeft(animationType->cornerLeftBottomFilePath);
        builder.setCornerBottomRight(animationType->cornerRightBottomFilePath);
        builder.setMiddleTop(animationType->middleTopFilePath);
        builder.setBottomMiddle(animationType->middleBottomFilePath);
        builder.setRightMiddle(animationType->middleRightFilePath);
        builder.setLeftMiddle(animationType->middleLeftFilePath);
        builder.setCenter(animationType->centerFilePath);
        builder.setZIndex(graphic.zIndex);
        builder.render();
    } else if (dynamic_cast<PlatformSingleLayerType*>(type)) {
        PlatformSingleLayerType* animationType = dynamic_cast<PlatformSingleLayerType*>(type);
        PlatformSingleFactory builder = PlatformSingleFactory(graphic.cols, animationType->spriteSize, graphic.scale, graphic.transparency, graphic.position);
        builder.setCornerLeft(animationType->cornerLeftFilePath);
        builder.setCornerRight(animationType->cornerRightFilePath);
        builder.setMiddle(animationType->centerFilePath);
        builder.setZIndex(graphic.zIndex);
        builder.render();
    }
}

void StageFactory::addSpritesToGraphic(jimp::AnimatedGraphic *animatedGraphic, jimp::AnimationType *animationType) {
    animatedGraphic->lockForDeletion();
    if (GameEngine::getInstance()->isRunning()) {
        for (const auto& subAnimation : animationType->subAnimations) {
            for (int i = 0; i < subAnimation->spriteCount; i++) {
                if (stageLoadInterrupted) {
                    animatedGraphic->unlockForDeletion();
                    return;
                }
                animatedGraphic->addSprite(subAnimation->name, std::regex_replace(subAnimation->filePath, std::regex("\\{i\\}"), std::to_string(i)));
            }
            animatedGraphic->setSpriteSwapInterval(subAnimation->name, subAnimation->spriteSwapInterval);
        }
    }
    animatedGraphic->unlockForDeletion();
}

void StageFactory::manageSpriteLoaderThreads() {
    while (!stageLoadStopped || spriteLoaderThreads->size() > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        
        spriteLoaderThreads->erase(std::remove_if(spriteLoaderThreads->begin(), spriteLoaderThreads->end(),
                               [](std::thread* threadToCheck) {
            threadToCheck->join();
            delete threadToCheck;
            return true;
        }), spriteLoaderThreads->end());
    }
    threadManagerStopped = true;
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
