#ifndef sprite_hpp
#define sprite_hpp

#include <string>
#include <SFML/Graphics.hpp>

namespace jimp {

class Sprite {
    
private:
    int x;
    int y;
    std::string filePath;
    sf::Image* image;
    
protected:
    void loadImage(std::string filePath);
    std::string getFilePath();
    
public:
    Sprite(int x, int y, std::string filePath);
    virtual int getX();
    virtual void setX(int x);
    virtual int getY();
    virtual void setY(int y);
    void setPosition(int x, int y);
    sf::Image& getImage();
};
}

#endif
