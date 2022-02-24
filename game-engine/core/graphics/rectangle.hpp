#ifndef rectangle_hpp
#define rectangle_hpp

#include "shape.hpp"

namespace jimp {

class Rectangle : Shape {
    
public:
    sf::Shape getShape() override;
    
};

}

#endif
