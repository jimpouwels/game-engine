#ifndef sprite_hpp
#define sprite_hpp

#include <string>
#include <SFML/Graphics.hpp>

namespace jimp {

class Sprite {
    
private:
    float x;
    float y;
    std::string filePath;
    sf::Image* image;
    
protected:
    void loadImage(std::string filePath);
    std::string getFilePath();
    
public:
    Sprite(float x, float y, std::string filePath);
    virtual float getX();
    virtual void setX(float x);
    virtual float getY();
    virtual void setY(float y);
    void setPosition(float x, float y);
    sf::Image& getImage();
};
}

#endif
