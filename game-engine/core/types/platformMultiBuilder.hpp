#ifndef platformMultiBuilder_hpp
#define platformMultiBuilder_hpp

#include <string>
#include "vector2D.hpp"

namespace jimp {

class PlatformMultiBuilder {
    
private:
    jimp::Vector2D position;
    int rowCount = 0;
    int columnCount = 0;
    float blockSize = 0.0F;
    float scale = 1.0F;
    std::string cornerLeftTopFilePath;
    std::string cornerRightTopFilePath;
    std::string cornerLeftBottomFilePath;
    std::string cornerRightBottomFilePath;
    std::string middleTopFilePath;
    std::string middleBottomFilePath;
    std::string middleRightFilePath;
    std::string middleLeftFilePath;
    std::string centerFilePath;
    
public:
    PlatformMultiBuilder(uint16_t rowCount, uint16_t columnCount, float blockSize, float scale, jimp::Vector2D position);
    jimp::Vector2D getPosition();
    void render();
    float getWidth();
    float getHeight();
    void setCornerTopLeft(std::string cornerLeftTopFilePath);
    void setCornerTopRight(std::string cornerRightTopFilePath);
    void setCornerBottomLeft(std::string cornerLeftBottomFilePath);
    void setCornerBottomRight(std::string cornerRightBottomFilePath);
    void setMiddleTop(std::string middleTopFilePath);
    void setBottomMiddle(std::string middleBottomFilePath);
    void setRightMiddle(std::string middleRightFilePath);
    void setLeftMiddle(std::string middleLeftFilePath);
    void setCenter(std::string centerFilePath);
};

}

#endif
