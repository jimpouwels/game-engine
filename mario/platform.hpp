#ifndef platform_hpp
#define platform_hpp

#include <string>
#include "vector2D.hpp"

namespace ghosty {

class Platform {
    
private:
    jimp::Vector2D position;
    
public:
    Platform(std::string cornerBlockFilePath, std::string innerBlockFilePath, uint16_t rowCount, uint16_t columnCount, float blockSize, float scale, jimp::Vector2D position);
    jimp::Vector2D getPosition();
};

}

#endif
