#include "tile.hpp"
#include "animatedGraphic.hpp"

namespace mario {

jimp::AnimatedGraphic* Tile::getAnimatedGraphic() {
    return animatedGraphic;
}

void Tile::add(jimp::AnimatedGraphic* animatedGraphic) {
    this->animatedGraphic = animatedGraphic;
}

}
