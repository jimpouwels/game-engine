#include "gameEngine.h"
#include "drawable.h"
#include <SFML/Graphics.hpp>
#include <chrono>

namespace jimp {

GameEngine::GameEngine(int screenWidth, int screenHeight, std::string windowTitle, int desiredFrameRate, float pixelSize) {
    this->screenWidth = screenWidth * pixelSize;
    this->screenHeight = screenHeight * pixelSize;
    this->frameRate = desiredFrameRate;
    this->timePerFrame = 1.0 / desiredFrameRate;
    this->pixelSize = pixelSize;
    window = new sf::RenderWindow(sf::VideoMode(this->getScreenWidth(), this->getScreenHeight()), windowTitle);
}

GameEngine::~GameEngine() {
    delete this->window;
}

void GameEngine::start() {
    while (window->isOpen()) {
        sf::Event event;
        
        while (window->pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window->close();
                    break;
            }
        }
        
        std::chrono::time_point<std::chrono::system_clock> currentTime = std::chrono::system_clock::now();
        std::chrono::duration<float> elapsedTime = (currentTime - previousFrameTime);
        
        if (elapsedTime.count() > timePerFrame) {
            window->clear();
            onFrame(elapsedTime.count());
            window->display();
            previousFrameTime = currentTime;
        }
    }
}

void GameEngine::draw(jimp::Drawable* drawable) {
    sf::Texture sfmlTexture;
    sfmlTexture.loadFromFile(drawable->getFilePath());
    sf::Sprite sfmlSprite;
    sfmlSprite.setTexture(sfmlTexture);
    sfmlSprite.setPosition(drawable->getX(), drawable->getY());
    sfmlSprite.setScale(this->pixelSize, this->pixelSize);
    window->draw(sfmlSprite);
    delete drawable;
}

int GameEngine::getScreenWidth() {
    return this->screenWidth;
}

int GameEngine::getScreenHeight() {
    return this->screenHeight;
}

}
