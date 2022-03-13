#include "mapLoader.hpp"
#include "json.hpp"

namespace jimp {

using json = nlohmann::json;

std::list<AnimatedGraphic*>* MapLoader::loadGraphics() {
    std::list<AnimatedGraphic*>* graphics = new std::list<AnimatedGraphic*>;
    
    return graphics;
}

}
