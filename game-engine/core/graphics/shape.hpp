#ifndef shape_hpp
#define shape_hpp

#include <SFML/Graphics.hpp>
#include "drawable.hpp"

namespace jimp {

class Shape : public Drawable {
    
private:
    uint32_t color;
    
public:
    Shape(uint32_t color, float scale, uint16_t repeat);
    ~Shape();
    uint32_t getColor();
    int getWidth() override;
};

}

#endif
