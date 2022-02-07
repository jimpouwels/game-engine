#ifndef timing_hpp
#define timing_hpp

namespace jimp {

class Timing {
    
public:
    static float toValueForElapsedTime(float valuePerSecond, float elapsedTime) {
        return valuePerSecond / (1.0F / elapsedTime);
    }
    
};

}

#endif
