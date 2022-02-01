#ifndef sprite_hpp
#define sprite_hpp

#include <string>
#include <SFML/Graphics.hpp>

#include "screen.hpp"
#include "point2D.hpp"

namespace jimp {

class Sprite {
    
private:
    Point2D position = { .x = 0, .y = 0};
    float scale = 1.0F;
    float angle = 0.0F;
    jimp::Screen* screen;
    std::string filePath;
    sf::Image* image;
    
protected:
    void loadImage(std::string filePath);
    
public:
    Sprite(jimp::Screen* screen, float x, float y, float scale, std::string filePath);
    Sprite(jimp::Screen* screen, float x, float y, float scale, int angle, std::string filePath);
    ~Sprite();
    Point2D& getPosition();
    void setPosition(Point2D position);
    void setX(float x);
    void setY(float y);
    float getScale();
    void setScale(float scale);
    float getRotationAngle();
    void setRotationAngle(float angle);
    int getWidth();
    int getHeight();
    bool isOutsideScreenBelow();
    bool isOutsideScreenAbove();
    bool isOutsideScreenLeft();
    bool isOutsideScreenRight();
    void setPosition(float x, float y);
    std::string getFilePath();
    sf::Image& getImage();
};
}

#endif
