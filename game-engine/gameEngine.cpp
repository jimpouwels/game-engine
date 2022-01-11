#include <SFML/Graphics.hpp>
#include <chrono>

#include "gameEngine.hpp"
#include "keyboardHandler.hpp"

namespace jimp {

GameEngine::GameEngine(int screenWidth, int screenHeight, std::string windowTitle, int desiredFrameRate, float pixelSize) {
    this->screenWidth = screenWidth * pixelSize;
    this->screenHeight = screenHeight * pixelSize;
    this->frameRate = desiredFrameRate;
    this->timePerFrame = 1.0 / desiredFrameRate;
    this->pixelSize = pixelSize;
    this->windowTitle = windowTitle;
    window = new sf::RenderWindow(sf::VideoMode(this->getScreenWidth(), this->getScreenHeight()), windowTitle);
    keyboardHandler = new jimp::KeyboardHandler();
}

GameEngine::~GameEngine() {
    delete window;
    delete keyboardHandler;
}

void GameEngine::start() {
    while (window->isOpen()) {
        sf::Event event;
        
        while (window->pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window->close();
                    break;
                case sf::Event::KeyPressed:
                    keyboardHandler->handleEvent(event);
                    break;
                case sf::Event::KeyReleased:
                    keyboardHandler->handleEvent(event);
                    break;
            }
        }
        
        std::chrono::time_point<std::chrono::system_clock> currentTime = std::chrono::system_clock::now();
        std::chrono::duration<float> elapsedTimeSincePreviousFrame = (currentTime - previousFrameTime);
        std::chrono::duration<float> elapsedTimeSincePreviousFpsMeasurement = (currentTime - previousFpsMeasurementTime);
        
        if (elapsedTimeSincePreviousFrame.count() > timePerFrame || frameRate == -1) {
            window->clear();
            onFrame(elapsedTimeSincePreviousFrame.count());
            window->display();
            previousFrameTime = currentTime;
            
            totalFrames++;
            if (elapsedTimeSincePreviousFpsMeasurement.count() > 5) {
                previousFpsMeasurementTime = currentTime;
                totalFrames = 0;
            }
        }
        
        window->setTitle(windowTitle + " FPS: " + std::to_string(totalFrames / elapsedTimeSincePreviousFpsMeasurement.count()));
    }
}

void GameEngine::draw(jimp::Sprite& sprite) {
    sf::Texture sfmlTexture;
    sfmlTexture.loadFromImage(sprite.getImage());
    sf::Sprite sfmlSprite;
    sfmlSprite.setTexture(sfmlTexture);
    sfmlSprite.setPosition(sprite.getX(), sprite.getY());
    sfmlSprite.setScale(pixelSize, pixelSize);
    window->draw(sfmlSprite);
}

void GameEngine::addKeyListener(KeyListener* keyListener) {
    keyboardHandler->addKeyListener(keyListener);
}

int GameEngine::getScreenWidth() {
    return this->screenWidth;
}

int GameEngine::getScreenHeight() {
    return this->screenHeight;
}

}
