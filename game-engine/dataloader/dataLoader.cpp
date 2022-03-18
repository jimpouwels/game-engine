#include "dataLoader.hpp"
#include "json.hpp"
#include "platformMultiLayerType.hpp"
#include "platformSingleLayerType.hpp"
#include "animationType.hpp"
#include "color.hpp"

#include <string>
#include <fstream>
#include <streambuf>

namespace jimp {

DataLoader::DataLoader(std::string typesFilePath) {
    this->typesFilePath = typesFilePath;
}

std::list<Type*>* DataLoader::loadTypes() {
    std::list<Type*>* types = new std::list<Type*>;
    std::string typesJsonString = loadFileContents(typesFilePath);
    nlohmann::json typesJson = nlohmann::json::parse(typesJsonString);
    nlohmann::json t = typesJson.at("types");
    
    for (int i = 0; i < t.size(); i++) {
        nlohmann::json typeJson = t.at(i);
        Type* type = createType(typeJson);
        type->name = typeJson.at("name");
        type->base = typeJson.at("base");
        if (typeJson.contains("spriteSwapInterval")) {
            type->spriteSwapInterval = typeJson.at("spriteSwapInterval");
        }
        types->push_back(type);
    }
    return types;
}

std::list<Graphic> DataLoader::loadGraphics(std::string filePath) {
    std::list<Graphic> graphics = std::list<Graphic>();
    std::string graphicsJsonString = loadFileContents(filePath);
    nlohmann::json graphicsJson = nlohmann::json::parse(graphicsJsonString);
    nlohmann::json g = graphicsJson.at("graphics");
    
    for (int i = 0; i < g.size(); i++) {
        nlohmann::json graphicJson = g.at(i);
        Graphic graphic = Graphic();
        graphic.description = graphicJson.at("description");
        graphic.type = graphicJson.at("type");
        graphic.position = Vector2D::from(graphicJson.at("position").at("x"), graphicJson.at("position").at("y"));
        if (graphicJson.contains("scale")) {
            graphic.scale = graphicJson.at("scale");
        }
        if (graphicJson.contains("applyGravity")) {
            graphic.applyGravity = graphicJson.at("applyGravity");
        }
        if (graphicJson.contains("rotationAngle")) {
            graphic.rotationAngle = graphicJson.at("rotationAngle");
        }
        if (graphicJson.contains("applyScrolling")) {
            graphic.applyScrolling = graphicJson.at("applyScrolling");
        }
        if (graphicJson.contains("rows")) {
            graphic.rows = graphicJson.at("rows");
        }
        if (graphicJson.contains("cols")) {
            graphic.cols= graphicJson.at("cols");
        }
        if (graphicJson.contains("isMainCharacter")) {
            graphic.isMainCharacter = graphicJson.at("isMainCharacter");
        }
        if (graphicJson.contains("zIndex")) {
            graphic.zIndex = graphicJson.at("zIndex");
        }
        if (graphicJson.contains("collidable")) {
            graphic.collidable = graphicJson.at("collidable");
        }
        if (graphicJson.contains("rgb")) {
            graphic.rgb = Color::from(graphicJson.at("rgb"));
        }
        if (graphicJson.contains("animateRgb")) {
            nlohmann::json rgbJson = graphicJson.at("animateRgb");
            if (rgbJson.contains("from") && rgbJson.contains("to") && rgbJson.contains("seconds")) {
                graphic.animateRgb = true;
                graphic.animateRgbFrom = Color::from(rgbJson.at("from"));
                graphic.animateRgbTo = Color::from(rgbJson.at("to"));
                graphic.animateRgbSeconds = rgbJson.at("seconds");
            }
        }
        graphics.push_back(graphic);
    }
    
    return graphics;
}

std::string DataLoader::loadFileContents(std::string filePath) {
    std::ifstream t(filePath);
    return std::string((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
}

Type* DataLoader::createType(nlohmann::json typeJson) {
    if (typeJson.at("base") == "platform-multi-layer") {
        PlatformMultiLayerType* type = new PlatformMultiLayerType();
        nlohmann::json sprites = typeJson.at("sprites");
        type->cornerLeftTopFilePath = sprites.at("cornerLeftTop");
        type->cornerRightTopFilePath = sprites.at("cornerRightTop");
        type->cornerRightTopFilePath = sprites.at("cornerRightTop");
        type->cornerLeftBottomFilePath = sprites.at("cornerLeftBottom");
        type->cornerRightBottomFilePath = sprites.at("cornerRightBottom");
        type->middleTopFilePath = sprites.at("middleTop");
        type->middleBottomFilePath = sprites.at("middleBottom");
        type->middleRightFilePath = sprites.at("middleRight");
        type->middleLeftFilePath = sprites.at("middleLeft");
        type->centerFilePath = sprites.at("center");
        type->spriteSize = typeJson.at("spriteSize");
        return type;
    } else if (typeJson.at("base") == "platform-single-layer") {
        PlatformSingleLayerType* type = new PlatformSingleLayerType();
        nlohmann::json sprites = typeJson.at("sprites");
        type->cornerLeftFilePath = sprites.at("cornerLeft");
        type->cornerRightFilePath = sprites.at("cornerRight");
        type->centerFilePath = sprites.at("center");
        type->spriteSize = typeJson.at("spriteSize");
        return type;
    } else if (typeJson.at("base") == "animation") {
        AnimationType* type = new AnimationType();
        if (typeJson.contains("custom")) {
            type->custom = typeJson.at("custom");
        }
        nlohmann::json subAnimationsJson = typeJson.at("subAnimations");
        for (int i = 0; i < subAnimationsJson.size(); i++) {
            nlohmann::json subAnimationJson = subAnimationsJson.at(i);
            SubAnimation* subAnimation = new SubAnimation();
            subAnimation->name = subAnimationJson.at("name");
            subAnimation->filePath = subAnimationJson.at("filePath");
            subAnimation->spriteCount = subAnimationJson.at("count");
            type->subAnimations.push_back(subAnimation);
        }
        return type;
    } else {
        return nullptr;
    }
}

}
