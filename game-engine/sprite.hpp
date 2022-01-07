#ifndef sprite_hpp
#define sprite_hpp

#include <string>

namespace jimp {

class Sprite {
    
private:
    int x;
    int y;
    std::string filePath;
    
public:
    Sprite(int x, int y, std::string filePath);
    virtual int getX();
    virtual int getY();
    virtual std::string getFilePath();
    
};
}

#endif
