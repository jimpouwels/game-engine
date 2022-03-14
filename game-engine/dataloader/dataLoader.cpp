#include "dataLoader.hpp"
#include "json.hpp"
#include "platformMultiLayerType.hpp"
#include "platformSingleLayerType.hpp"

#include <string>
#include <fstream>
#include <streambuf>

namespace jimp {

using json = nlohmann::json;

DataLoader::DataLoader(std::string typesFilePath) {
    this->typesFilePath = typesFilePath;
}

std::list<Type*>* DataLoader::loadTypes() {
    std::list<Type*>* types = new std::list<Type*>;
    std::string typesJsonString = loadFileContents(typesFilePath);
    json typesJson = json::parse(typesJsonString);
    json t = typesJson.at("types");
    for (int i = 0; i < t.size(); i++) {
        json typeJson = t.at(i);
        Type* type = loadType(typeJson);
        types->push_back(type);
        
    }
    return types;
}

std::list<Graphic*>* DataLoader::loadGraphics(std::string filePath) {
    std::list<Graphic*>* graphics = new std::list<Graphic*>;
    
    return graphics;
}

std::string DataLoader::loadFileContents(std::string filePath) {
    std::ifstream t(filePath);
    std::string str((std::istreambuf_iterator<char>(t)),
                     std::istreambuf_iterator<char>());
    return str;
}

Type* DataLoader::loadType(json typeJson) {
    if (typeJson.at("base") == "platform-multi-layer") {
        PlatformMultiLayerType* type = new PlatformMultiLayerType();
        json sprites = typeJson.at("sprites");
        type->cornerLeftTopFilePath = sprites.at("cornerLeftTop");
        type->cornerRightTopFilePath = sprites.at("cornerRightTop");
        type->cornerRightTopFilePath = sprites.at("cornerRightTop");
        type->cornerLeftBottomFilePath = sprites.at("cornerLeftBottom");
        type->cornerRightBottomFilePath = sprites.at("cornerRightBottom");
        type->middleTopFilePath = sprites.at("middleTop");
        type->middleBottomFilePath = sprites.at("middleBottom");
        type->middleRightFilePath = sprites.at("middleRight");
        type->middleLeftFilePath = sprites.at("middleLeft");
        
        enrichtWithTypeBase(type, typeJson);
        return type;
    } else {
        return nullptr;
    }
}

void DataLoader::enrichtWithTypeBase(Type* type, json typeJson) {
    type->name = typeJson.at("name");
    type->base = typeJson.at("base");
}

}
