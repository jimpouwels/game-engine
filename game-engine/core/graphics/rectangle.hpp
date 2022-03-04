#ifndef rectangle_hpp
#define rectangle_hpp

#include "shape.hpp"

namespace jimp {

class Rectangle : public Shape {
    
private:
    float width;
    float height;
    
public:
    Rectangle(int width, int height, float scale, uint32_t color, uint16_t repeat);
    int getSingleWidth() override;
    int getHeight() override;
    
};

}

#endif
