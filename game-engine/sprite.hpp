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
    virtual void setX(int x);
    virtual int getY();
    virtual void setY(int y);
    virtual std::string getFilePath();
    
};
}

#endif
