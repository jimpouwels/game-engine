#include "ghostyStageFactory.hpp"
#include "ghosty.hpp"
#include "movingPlatform.hpp"

namespace ghosty {

GhostyStageFactory::GhostyStageFactory(std::string typesFilePath) : jimp::StageFactory(typesFilePath) {
}

jimp::AnimatedGraphic* GhostyStageFactory::createCustomGraphic(std::string name) {
    if (name == "ghosty") {
        return new Ghosty();
    } else if (name == "ghosty-moving-platform") {
        return new MovingPlatform();
    }
    return nullptr;
}

}
