#ifndef type_hpp
#define type_hpp

#include <string>

namespace jimp {

struct Type {
    std::string name;
    std::string base;
    virtual ~Type() { };
};

}

#endif
