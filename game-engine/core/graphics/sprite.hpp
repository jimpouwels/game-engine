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
    Sprite(float x, float y, float scale, std::string filePath);
    Sprite(float x, float y, float scale, uint16_t angle, std::string filePath);
    ~Sprite();
    int getWidth() override;
    int getHeight() override;
    jimp::Image& getImage();
};
}

#endif
