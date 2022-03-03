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
    
public:
    Sprite(float x, float y, float scale, uint16_t angle, uint16_t repeat, std::string filePath);
    ~Sprite();
    jimp::Image& getImage();
    int getSingleWidth();
    int getWidth() override;
    int getHeight() override;
};
}

#endif
