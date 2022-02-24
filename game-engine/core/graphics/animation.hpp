#ifndef animation_hpp
#define animation_hpp

#include <vector>
#include <list>
#include <string>
#include "drawable.hpp"

namespace jimp {

class Animation {

private:
    std::string id;
    std::vector<Drawable*>* drawables;
    uint16_t activeDrawableIndex = 0;
    
public:
    Animation(std::string id);
    ~Animation();
    std::string getId();
    void addDrawable(Drawable* drawable);
    long getNumberOfDrawables();
    Drawable* getActiveDrawable();
    std::list<Drawable*> getAllDrawables();
    void switchToNextDrawable();
    
};

}

#endif
