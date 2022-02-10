#ifndef sound_hpp
#define sound_hpp

#include <SFML/Audio.hpp>

namespace jimp {

class Sound {

private:
    sf::SoundBuffer* buffer = nullptr;
    sf::Sound* sound = nullptr;
    
public:
    Sound(std::string filePath);
    ~Sound();
    void play(uint16_t volume);
    void stop();
    void loop(uint16_t volume);
    void setVolume(uint16_t volume);
    uint16_t getVolume();
    
};

}

#endif
