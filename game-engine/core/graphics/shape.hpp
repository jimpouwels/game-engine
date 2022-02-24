#ifndef rectangle_hpp
#define rectangle_hpp

#include <SFML/Graphics.hpp>
#include "drawable.hpp"

namespace jimp {

class Shape : Drawable {
    
public:
    virtual sf::Shape getShape() = 0;

};

}

#endif
