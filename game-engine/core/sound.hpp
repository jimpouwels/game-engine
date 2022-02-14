#ifndef sound_hpp
#define sound_hpp

#include <SFML/Audio.hpp>
#include <list>
#include <chrono>

namespace jimp {

class Sound {

private:
    sf::SoundBuffer* buffer = nullptr;
    sf::Sound* sound = nullptr;
    bool isFadingOut = false;
    std::chrono::time_point<std::chrono::system_clock> fadeOutEndTime;
    std::list<sf::Sound*>* fullRuns = nullptr;
    void updateFadeOut(float elapsedTime);
    void stopFadeOut();
    
public:
    Sound(std::string filePath);
    ~Sound();
    void onUpdate(float elapsedTime);
    void play(uint16_t volume);
    void playTillEnd(uint16_t volume);
    void fadeOut(uint16_t durationInSeconds);
    void stop();
    void loop(uint16_t volume);
    void setVolume(float volume);
    float getVolume();
    void cleanupCompletedFullRunRuns();
    
};

}

#endif
