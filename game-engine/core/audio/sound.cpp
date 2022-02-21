#include "sound.hpp"
#include "timing.hpp"
#include "gameEngine.hpp"
#include <iostream>

namespace jimp {

Sound::Sound(std::string filePath) {
    this->filePath = filePath;
    this->fullRuns = new std::list<sf::Sound*>;
    this->buffer = new sf::SoundBuffer();
    this->buffer->loadFromFile(filePath);
    this->sound = new sf::Sound();
    this->sound->setBuffer(*buffer);
    GameEngine::getInstance()->registerSound(this);
}

Sound::~Sound() {
    sound->stop();
    delete sound;
}

void Sound::onFrame(float elapsedTime) {
    updateFadeOut(elapsedTime);
}

void Sound::fadeOut(uint16_t durationInSeconds) {
    isFadingOut = true;
    fadeOutEndTime = std::chrono::system_clock::now() + std::chrono::seconds(durationInSeconds);
}

void Sound::play(uint16_t volume) {
    stopFadeOut();
    this->sound->stop();
    this->sound->setVolume(volume);
    this->sound->play();
}

void Sound::playTillEnd(uint16_t volume) {
    stopFadeOut();
    sf::Sound* fullRun = new sf::Sound();
    fullRun->setBuffer(*buffer);
    fullRun->setVolume(volume);
    fullRun->play();
    fullRuns->push_back(fullRun);
}

void Sound::stop() {
    stopFadeOut();
    this->sound->stop();
}

void Sound::setVolume(float volume) {
    this->sound->setVolume(volume);
}

float Sound::getVolume() {
    return this->sound->getVolume();
}

std::string Sound::getFilePath() {
    return filePath;
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

void Sound::updateFadeOut(float elapsedTime) {
    if (isFadingOut && sound->getStatus() == 2) {
        float remainingFadeOutTimeInMillis = std::chrono::duration_cast<std::chrono::milliseconds>(fadeOutEndTime-std::chrono::system_clock::now()).count();
        
        if (remainingFadeOutTimeInMillis <= 0) {
            stop();
            stopFadeOut();
        } else {
            float fadeOutFactor = remainingFadeOutTimeInMillis / (elapsedTime * 1000);
            float newVolume = getVolume() - (getVolume() / fadeOutFactor);
            setVolume(newVolume);
        }
    } else if (sound->getStatus() == 0) {
        stopFadeOut();
    }
}

void Sound::stopFadeOut() {
    isFadingOut = false;
}

}
