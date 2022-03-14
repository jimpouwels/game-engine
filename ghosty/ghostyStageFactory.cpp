#include "ghostyStageFactory.hpp"
#include "ghosty.hpp"

namespace ghosty {

GhostyStageFactory::GhostyStageFactory(std::string typesFilePath) : jimp::StageFactory(typesFilePath) {
}

jimp::AnimatedGraphic* GhostyStageFactory::createCustomGraphic(std::string name) {
    if (name == "ghosty") {
        return new Ghosty(jimp::Vector2D::empty());
    }
    return nullptr;
}

}
