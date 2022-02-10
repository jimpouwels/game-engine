#include "sound.hpp"

namespace jimp {

Sound::Sound(std::string filePath) {
    this->buffer = new sf::SoundBuffer();
    this->buffer->loadFromFile(filePath);
    this->sound = new sf::Sound();
    this->sound->setBuffer(*buffer);
}

Sound::~Sound() {
    delete sound;
}

void Sound::play(uint16_t volume) {
    this->sound->stop();
    this->sound->setVolume(volume);
    this->sound->play();
}

void Sound::stop() {
    this->sound->stop();
}

void Sound::setVolume(uint16_t volume) {
    this->sound->setVolume(volume);
}

uint16_t Sound::getVolume() {
    return this->sound->getVolume();
}

void Sound::loop(uint16_t volume) {
    this->sound->stop();
    this->sound->setLoop(true);
    play(volume);
}

}
