#ifndef rectangle_hpp
#define rectangle_hpp

#include "shape.hpp"

namespace jimp {

class Rectangle : public Shape {
    
private:
    float width;
    float height;
    
public:
    Rectangle(int width, int height, uint32_t color);
    int getWidth() override;
    int getHeight() override;
    
};

}

#endif
