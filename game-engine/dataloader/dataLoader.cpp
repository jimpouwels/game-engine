#include "dataLoader.hpp"
#include "json.hpp"

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
    std::cout << typesJson.at("types").at(0).at("name");
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

}
