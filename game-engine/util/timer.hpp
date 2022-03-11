#ifndef timer_hpp
#define timer_hpp

#include <stdio.h>

namespace jimp {

class Timer {
    
private:
    
    
public:
    Timer();
    void onUpdate(float elapsedTime);
    static Timer* getInstance();
};

}

#endif
