#ifndef sprite_hpp
#define sprite_hpp

#include <string>

#include "vector2D.hpp"
#include "image.hpp"
#include "drawable.hpp"
#include "color.hpp"

namespace jimp {

class Sprite : public Drawable {
    
private:
    Image* image;
    int marginLeft = 0;
    int marginRight = 0;
    int marginTop = 0;
    int marginBottom = 0;
    bool inversedHorizontally = false;
    std::string filePath;
    Color rgb = Color::full();
    void crop();
    
public:
    Sprite(float x, float y, float scale, uint16_t angle, uint16_t repeat, std::string filePath);
    ~Sprite();
    jimp::Image& getImage();
    void setDrawInversedHorizontally(bool inversed);
    bool drawInversedHorizontally();
    int getMarginLeft();
    int getMarginRight();
    int getMarginTop();
    int getMarginBottom();
    int getSingleWidth() override;
    int getWidth() override;
    int getHeight() override;
    void setRgbLevels(Color rgb);
    Color getRgbLevels();
    std::string getFilePath();
};
}

#endif
