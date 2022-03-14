#ifndef platformSingleLayerType_hpp
#define platformSingleLayerType_hpp

#include "type.hpp"

namespace jimp {

struct PlatformSingleLayerType : Type {
    std::string cornerLeftFilePath;
    std::string cornerRightFilePath;
    std::string centerFilePath;
};

}

#endif
