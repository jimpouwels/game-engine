#include "dataLoader.hpp"
#include "json.hpp"

namespace jimp {

using json = nlohmann::json;

DataLoader::DataLoader(std::string typesFilePath) {
    this->typesFilePath = typesFilePath;
}

std::list<Type*>* DataLoader::loadTypes() {
    std::list<Type*>* types = new std::list<Type*>;
    
    return types;
}

std::list<Graphic*>* DataLoader::loadGraphics(std::string filePath) {
    std::list<Graphic*>* graphics = new std::list<Graphic*>;
    
    return graphics;
}

}
