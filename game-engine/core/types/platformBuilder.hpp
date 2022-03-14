#ifndef platform_hpp
#define platform_hpp

#include <string>
#include "vector2D.hpp"

namespace jimp {

class PlatformBuilder {
    
private:
    jimp::Vector2D position;
    int rowCount = 0;
    int columnCount = 0;
    float blockSize = 0.0F;
    float scale = 1.0F;
    
public:
    PlatformBuilder(std::string cornerBlockFilePath, std::string innerBlockFilePath, uint16_t rowCount, uint16_t columnCount, float blockSize, float scale, jimp::Vector2D position);
    jimp::Vector2D getPosition();
    float getWidth();
    float getHeight();
};

}

#endif
