#include "tileMap.hpp"
#include "tile.hpp"

namespace mario {

TileMap::TileMap(uint16_t rows, uint16_t columns) {
    this->columnCount = columns;
    this->rowCount = rows;
    this->tiles = new std::vector<Tile*>;
    for (int i = 0; i < rowCount * columnCount; i++) {
        tiles->push_back(new Tile());
    }
}

TileMap::~TileMap() {
    for (int i = 0; i < tiles->size(); i++) {
        delete tiles->at(i);
    }
    delete tiles;
}

Tile* TileMap::getTileAt(uint16_t x, uint16_t y) {
    return tiles->at(columnCount * y + x);
}

uint16_t TileMap::getColumnCount() {
    return columnCount;
}

uint16_t TileMap::getRowCount() {
    return rowCount;
}

}
