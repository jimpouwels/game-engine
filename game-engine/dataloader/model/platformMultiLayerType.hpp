#ifndef platformMultiLayerType_hpp
#define platformMultiLayerType_hpp

#include "type.hpp"

namespace jimp {

struct PlatformMultiLayerType : Type {
    std::string cornerLeftTopFilePath;
    std::string cornerRightTopFilePath;
    std::string cornerLeftBottomFilePath;
    std::string cornerRightBottomFilePath;
    std::string middleBottomFilePath;
    std::string middleTopFilePath;
    std::string middleRightFilePath;
    std::string middleLeftFilePath;
};

}

#endif
