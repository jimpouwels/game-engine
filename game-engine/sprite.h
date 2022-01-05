#ifndef sprite_h
#define sprite_h

#include <string>

namespace jimp {

class Sprite {
    
private:
    int x;
    int y;
    std::string filePath;
public:
    Sprite(int x, int y, std::string filePath);
    int getX();
    int getY();
    std::string getFilePath();
    
};
}

#endif
