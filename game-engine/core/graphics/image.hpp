#ifndef image_hpp
#define image_hpp

#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <string>

namespace jimp {

class Image {
  
private:
    sf::Image* image = nullptr;
    std::string filePath;
    void loadImage(std::string filePath);
    
public:
    Image(std::string filePath);
    ~Image();
    uint16_t getWidth();
    uint16_t getHeight();
    sf::Image& getImage();
    std::string getFilePath();
    
};

}

#endif
