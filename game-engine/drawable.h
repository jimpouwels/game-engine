#ifndef drawable_h
#define drawable_h

#include <string>

namespace jimp {

class Drawable {
  
public:
    virtual ~Drawable();
    virtual std::string getFilePath() = 0;
    virtual int getX() = 0;
    virtual int getY() = 0;

};
}

#endif
