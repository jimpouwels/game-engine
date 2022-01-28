#ifndef sprite_hpp
#define sprite_hpp

#include <string>
#include <SFML/Graphics.hpp>

namespace jimp {

class Sprite {
    
private:
    float x;
    float y;
    float scale = 1.0F;
    float rotationAngle = 0.0F;
    std::string filePath;
    sf::Image* image;
    
protected:
    void loadImage(std::string filePath);
    
public:
    Sprite(float x, float y, float scale, std::string filePath);
    Sprite(float x, float y, float scale, int rotationAngle, std::string filePath);
    ~Sprite();
    float getX();
    void setX(float x);
    float getY();
    void setY(float y);
    float getScale();
    float getRotationAngle();
    void setRotationAngle(float rotationAngle);
    int getWidth();
    int getHeight();
    void setPosition(float x, float y);
    std::string getFilePath();
    sf::Image& getImage();
};
}

#endif
