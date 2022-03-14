#ifndef animationType_hpp
#define animationType_hpp

#include "type.hpp"

namespace jimp {

struct SubAnimation {
    std::string name;
    std::string filePath;
    int spriteCount;
};

struct AnimationType : Type {
    bool custom = false;
    std::list<SubAnimation*> subAnimations = std::list<SubAnimation*>();
    
    ~AnimationType() {
        for (const auto& sub : subAnimations) {
            delete sub;
        }
    }
};

}

#endif
