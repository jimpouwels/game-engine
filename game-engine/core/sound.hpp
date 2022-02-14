#ifndef sound_hpp
#define sound_hpp

#include <SFML/Audio.hpp>
#include <list>

namespace jimp {

class Sound {

private:
    sf::SoundBuffer* buffer = nullptr;
    sf::Sound* sound = nullptr;
    std::list<sf::Sound*>* fullRuns = nullptr;
    
public:
    Sound(std::string filePath);
    ~Sound();
    void play(uint16_t volume);
    void playTillEnd(uint16_t volume);
    void stop();
    void loop(uint16_t volume);
    void setVolume(uint16_t volume);
    uint16_t getVolume();
    void cleanupCompletedFullRunRuns();
    
};

}

#endif
