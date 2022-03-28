#include "ghostyStageFactory.hpp"
#include "ghosty.hpp"
#include "movingPlatform.hpp"

namespace ghosty {

GhostyStageFactory::GhostyStageFactory(std::string typesFilePath) : jimp::StageFactory(typesFilePath) {
}

jimp::AnimatedGraphic* GhostyStageFactory::createCustomGraphic(std::string name, jimp::Graphic graphic) {
    if (name == "ghosty") {
        return new Ghosty();
    } else if (name == "ghosty-moving-platform") {
        jimp::Vector2D targetPosition = jimp::Vector2D::from(graphic.config.at("targetPosition").at("x"),
                                                             graphic.config.at("targetPosition").at("y"));
        return new MovingPlatform(graphic.position, targetPosition);
    }
    return nullptr;
}

}
