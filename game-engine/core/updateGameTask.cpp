#include "updateGameTask.hpp"
#include <iostream>
#include <chrono>

namespace jimp {

bool isUpdatingGame = false;

void doLoop(std::function<void(float)> callback) {
    isUpdatingGame = true;
    std::chrono::time_point<std::chrono::system_clock> previousUpdateTime;
    while (isUpdatingGame) {
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
        std::chrono::time_point<std::chrono::system_clock> currentTime = std::chrono::system_clock::now();
        std::chrono::duration<float> elapsed = currentTime - previousUpdateTime;
        previousUpdateTime = currentTime;
        callback(elapsed.count());
    }
}

UpdateGameTask::UpdateGameTask(std::function<void(float)> callback) {
    this->callback = callback;
}

UpdateGameTask::~UpdateGameTask() {
    isUpdatingGame = false;
    updateThread->join();
    delete updateThread;
}

void UpdateGameTask::start() {
    this->updateThread = new std::thread(doLoop, callback);
}

}
