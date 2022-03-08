#ifndef sprite_hpp
#define sprite_hpp

#include <string>

#include "vector2D.hpp"
#include "image.hpp"
#include "drawable.hpp"

namespace jimp {

class Sprite : public Drawable {
    
private:
    Image* image;
    bool showAsBlack = false;
    int marginLeft = 0;
    int marginRight = 0;
    int marginTop = 0;
    int marginBottom = 0;
    bool inversedHorizontally = false;
    void crop();
    
public:
    Sprite(float x, float y, float scale, uint16_t angle, uint16_t repeat, std::string filePath);
    ~Sprite();
    jimp::Image& getImage();
    void setDrawInversedHorizontally(bool inversed);
    bool drawInversedHorizontally();
    void setShowAsBlack(bool value);
    bool isSetShowAsBlack();
    int getMarginLeft();
    int getMarginRight();
    int getMarginTop();
    int getMarginBottom();
    int getSingleWidth() override;
    int getWidth() override;
    int getHeight() override;
};
}

#endif
