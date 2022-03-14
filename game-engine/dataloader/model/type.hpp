#ifndef type_hpp
#define type_hpp

#include <string>

namespace jimp {

struct Type {
    std::string name;
    std::string base;
    float spriteSwapInterval = -1.0f;
    bool isMainCharacter = false;
    virtual ~Type() { };
};

}

#endif
