#include "timer.hpp"

namespace jimp {

static Timer* timerInstance = nullptr;

Timer::Timer() {
    timerInstance = this;
}

Timer* Timer::getInstance() {
    return timerInstance;
}

void Timer::onUpdate(float elapsedTime) {
    
}

}
