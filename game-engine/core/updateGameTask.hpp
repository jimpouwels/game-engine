#ifndef updateGameTask_hpp
#define updateGameTask_hpp

#include <thread>

namespace jimp {

class UpdateGameTask {
    
private:
    std::thread* updateThread = nullptr;
    std::function<void(float)> callback;
    
public:
    UpdateGameTask(std::function<void(float)> callback);
    ~UpdateGameTask();
    void start();
    
};

}

#endif
