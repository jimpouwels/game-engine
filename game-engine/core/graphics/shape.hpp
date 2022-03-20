#ifndef shape_hpp
#define shape_hpp

#include <SFML/Graphics.hpp>
#include "drawable.hpp"
#include "color.hpp"

namespace jimp {

class Shape : public Drawable {
    
private:
    Color color;
    
public:
    Shape(Color color, float scale, uint16_t repeat);
    ~Shape();
    Color getColor();
    int getWidth() override;
};

}

#endif
