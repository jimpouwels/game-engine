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
    this->loadThreads = new std::list<std::thread*>();
    this->threadManager = new std::thread(&StageFactory::manageThreads, this);
}

StageFactory::~StageFactory() {
    threadManager->join();
    delete threadManager;
    delete dataLoader;
    for (const auto& type : *types) {
        delete type;
    }
    delete types;
    delete loadThreads;
}

void StageFactory::stopProcessing() {
    stageLoadInterrupted = true;
    while (!threadManagerStopped) {
    }
}

void StageFactory::loadStage(std::string filePath) {
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
            animatedGraphic = createCustomGraphic(type->name);
        } else {
            animatedGraphic = new AnimatedGraphic();
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
        loadThreads->push_back(new std::thread(&StageFactory::addSpritesToGraphic, this, animatedGraphic, animationType));
        GameEngine::getInstance()->registerGraphic(animatedGraphic);
    } else if (dynamic_cast<PlatformMultiLayerType*>(type)) {
        PlatformMultiLayerType* animationType = dynamic_cast<PlatformMultiLayerType*>(type);
        PlatformMultiFactory builder = PlatformMultiFactory(graphic.rows, graphic.cols, animationType->spriteSize, graphic.scale, graphic.position);
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
        PlatformSingleFactory builder = PlatformSingleFactory(graphic.cols, animationType->spriteSize, graphic.scale, graphic.position);
        builder.setCornerLeft(animationType->cornerLeftFilePath);
        builder.setCornerRight(animationType->cornerRightFilePath);
        builder.setMiddle(animationType->centerFilePath);
        builder.setZIndex(graphic.zIndex);
        builder.render();
    }
}

void StageFactory::addSpritesToGraphic(jimp::AnimatedGraphic *animatedGraphic, jimp::AnimationType *animationType) {
    animatedGraphic->lockForDeletion();
    for (const auto& subAnimation : animationType->subAnimations) {
        for (int i = 0; i < subAnimation->spriteCount; i++) {
            if (stageLoadInterrupted) {
                return;
            }
            animatedGraphic->addSprite(subAnimation->name, std::regex_replace(subAnimation->filePath, std::regex("\\{i\\}"), std::to_string(i)));
        }
    }
    animatedGraphic->unlockForDeletion();
}

void StageFactory::manageThreads() {
    while (!stageLoadInterrupted || loadThreads->size() > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        loadingSprites = loadThreads->size() > 0;
        std::list<std::thread*> threadsToDelete = std::list<std::thread*>();
        for (const auto& thread : *loadThreads) {
            thread->join();
            threadsToDelete.push_back(thread);
        }
        for (const auto& threadToDelete : threadsToDelete) {
            loadThreads->remove(threadToDelete);
            delete threadToDelete;
        }
    }
    threadManagerStopped = true;
}
     
bool StageFactory::isLoadingSprites() {
    return loadingSprites;
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
