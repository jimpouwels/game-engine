#ifndef mathUtils_h
#define mathUtils_h

#include <math.h>

namespace jimp {

class MathUtils {

public:
    static int randomNumberBetween(int from, int to) {
        return rand() % to + from;
    }
    
};

}

#endif
