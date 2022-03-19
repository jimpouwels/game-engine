#ifndef platformSingleFactory_hpp
#define platformSingleFactory_hpp

#include <string>
#include "vector2D.hpp"

namespace jimp {

class PlatformSingleFactory {
    
private:
    jimp::Vector2D position;
    int columnCount = 0;
    float blockSize = 0.0F;
    float scale = 1.0F;
    int zIndex = 0;
    float transparency = 100.0f;
    std::string cornerLeftFilePath;
    std::string cornerRightFilePath;
    std::string middleFilePath;
    
public:
    PlatformSingleFactory(uint16_t columnCount, float blockSize, float scale, float transparency, jimp::Vector2D position);
    jimp::Vector2D getPosition();
    void render();
    float getWidth();
    float getHeight();
    void setCornerLeft(std::string cornerLeftFilePath);
    void setCornerRight(std::string cornerRightFilePath);
    void setMiddle(std::string middleFilePath);
    void setZIndex(int zIndex);
};

}

#endif
