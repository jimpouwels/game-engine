#ifndef tileMap_hpp
#define tileMap_hpp

#include "tile.hpp"

namespace mario {

class TileMap {
  
private:
    uint16_t rowCount;
    uint16_t columnCount;
    std::vector<Tile*>* tiles = nullptr;
    
public:
    TileMap(uint16_t rows, uint16_t columns);
    ~TileMap();
    Tile* getTileAt(uint16_t x, uint16_t y);
    uint16_t getColumnCount();
    uint16_t getRowCount();
    
};

}

#endif
