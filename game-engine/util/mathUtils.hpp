#ifndef mathUtils_h
#define mathUtils_h

#include <math.h>

namespace jimp {

class MathUtils {

public:
    static int randomNumberBetween(int from, int to) {
        return rand() % (to - from) + from;
    }
    
    static bool isBetween(float value, float value1, float value2) {
        return value >= value1 && value <= value2;
    }
    
};

}

#endif
