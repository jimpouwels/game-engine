#include <SFML/Graphics.hpp>
#include <chrono>

#include "gameEngine.hpp"
#include "sprite.hpp"
#include "animatedSprite.hpp"
#include "keyboardHandler.hpp"

namespace jimp {

GameEngine::GameEngine(int screenWidth, int screenHeight, std::string windowTitle, int desiredFrameRate) {
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    this->frameRate = desiredFrameRate;
    this->timePerFrame = 1.0 / desiredFrameRate;
    this->windowTitle = windowTitle;
    window = new sf::RenderWindow(sf::VideoMode(this->getScreenWidth(), this->getScreenHeight()), windowTitle);
    keyboardHandler = new jimp::KeyboardHandler();
    this->previousFrameTime = std::chrono::system_clock::now();
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
        
        float elapsedTime = elapsedTimeSincePreviousFrame.count();
        if (elapsedTime > timePerFrame || frameRate == -1) {
            totalFrames++;
            drawFrame(elapsedTime);
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
    sfmlSprite.setPosition(sprite.getPosition().x, sprite.getPosition().y);
    sfmlSprite.setScale(sprite.getScale(), sprite.getScale());
    sf::Transform transform;
    transform.rotate(sprite.getRotationAngle(), sprite.getPosition().x + sprite.getWidth() / 2, sprite.getPosition().y + sprite.getHeight() / 2);
    window->draw(sfmlSprite, transform);
}

void GameEngine::addKeyListener(KeyListener* keyListener) {
    keyboardHandler->addKeyListener(keyListener);
}

int GameEngine::getScreenWidth() {
    return screenWidth;
}

int GameEngine::getScreenHeight() {
    return screenHeight;
}

bool GameEngine::isPositionWithinScreen(float x, float y) {
    return x <= getScreenWidth() && x >= 0 && y <= getScreenHeight() && y > 0;
}

float GameEngine::measureFps(std::chrono::time_point<std::chrono::system_clock>& currentTime) {
    std::chrono::duration<float> elapsedTimeSincePreviousMeasurement = (currentTime - previousFpsMeasurementTime);
    if (elapsedTimeSincePreviousMeasurement.count() > 5) {
        previousFpsMeasurementTime = currentTime;
        totalFrames = 0;
    }
    return totalFrames / elapsedTimeSincePreviousMeasurement.count();
}

void GameEngine::drawFrame(float elapsedTimeSincePreviousFrame) {
    window->clear();
    onFrame(elapsedTimeSincePreviousFrame);
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
