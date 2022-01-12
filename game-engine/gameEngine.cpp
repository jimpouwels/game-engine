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
        handleEvents();
        
        std::chrono::time_point<std::chrono::system_clock> currentTime = std::chrono::system_clock::now();
        std::chrono::duration<float> elapsedTimeSincePreviousFrame = (currentTime - previousFrameTime);
        
        if (elapsedTimeSincePreviousFrame.count() > timePerFrame || frameRate == -1) {
            totalFrames++;
            drawFrame(elapsedTimeSincePreviousFrame);
            previousFrameTime = currentTime;
        }
        
        window->setTitle(windowTitle + " FPS: " + std::to_string(measureFps(currentTime)));
    }
}

void GameEngine::draw(jimp::Sprite& sprite) {
    sf::Texture sfmlTexture;
    sfmlTexture.loadFromImage(sprite.getImage());
    sf::Sprite sfmlSprite;
    sfmlSprite.setTexture(sfmlTexture);
    sfmlSprite.setPosition(sprite.getX() * pixelSize, sprite.getY() * pixelSize);
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

float GameEngine::measureFps(std::chrono::time_point<std::chrono::system_clock>& currentTime) {
    std::chrono::duration<float> elapsedTimeSincePreviousMeasurement = (currentTime - previousFpsMeasurementTime);
    if (elapsedTimeSincePreviousMeasurement.count() > 5) {
        previousFpsMeasurementTime = currentTime;
        totalFrames = 0;
    }
    return totalFrames / elapsedTimeSincePreviousMeasurement.count();
}

void GameEngine::drawFrame(std::chrono::duration<float>& elapsedTimeSincePreviousFrame) {
    window->clear();
    onFrame(elapsedTimeSincePreviousFrame.count());
    window->display();
}

void GameEngine::handleEvents() {
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
}

}
