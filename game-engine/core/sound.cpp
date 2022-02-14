#include "sound.hpp"
#include <iostream>

namespace jimp {

Sound::Sound(std::string filePath) {
    this->fullRuns = new std::list<sf::Sound*>;
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

void Sound::playTillEnd(uint16_t volume) {
    sf::Sound* fullRun = new sf::Sound();
    fullRun->setBuffer(*buffer);
    fullRun->setVolume(volume);
    fullRun->play();
    fullRuns->push_back(fullRun);
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

void Sound::cleanupCompletedFullRunRuns() {
    std::list<sf::Sound*> toDelete = std::list<sf::Sound*>();
    for (const auto& fullRun: *fullRuns) {
        if (fullRun->getStatus() == 0) {
            toDelete.push_back(fullRun);
        }
    }
    for (const auto& fullRun: toDelete) {
        fullRuns->remove(fullRun);
        delete fullRun;
    }
}

}
