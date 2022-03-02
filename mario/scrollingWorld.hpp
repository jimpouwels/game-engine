#ifndef scrollingWorld_hpp
#define scrollingWorld_hpp

#include "animatedGraphic.hpp"
#include "tile.hpp"
#include "tileMap.hpp"

namespace mario {

class ScrollingWorld {
    
private:
    jimp::AnimatedGraphic* mainCharacter = nullptr;
    TileMap* tileMap = nullptr;
    uint16_t tileSize = 0;
    uint16_t startColumn = 0;
    int offsetX = 0;
    
public:
    ScrollingWorld(jimp::AnimatedGraphic* mainCharacter, int rows, int columns, uint16_t tileSize);
    ~ScrollingWorld();
    void addGraphic(int x, int y, jimp::AnimatedGraphic* animatedGraphic);
    void doOnUpdate();
    void doOnFrame();
};

}

#endif
