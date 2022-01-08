#ifndef sprite_hpp
#define sprite_hpp

#include <string>
#include <SFML/Graphics.hpp>

namespace jimp {

class Sprite {
    
private:
    int x;
    int y;
    sf::Image* image;
    
public:
    Sprite(int x, int y, std::string filePath);
    virtual int getX();
    virtual void setX(int x);
    virtual int getY();
    virtual void setY(int y);
    sf::Image& getImage();
};
}

#endif
