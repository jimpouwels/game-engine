#include <iostream>
#include <SFML/Graphics.hpp>
#include <chrono>
#include "sprite.h"

namespace jimp {

    class GameEngine {
        
    private:
        int screenWidth;
        int screenHeight;
        int frameRate;
        float timePerFrame;
        std::chrono::time_point<std::chrono::system_clock> previousFrameTime;
        sf::RenderWindow* window;
        
    public:
        GameEngine(int screenWidth, int screenHeight, std::string windowTitle, int desiredFrameRate);
        void start();
        void renderSprite(Sprite sprite);
        int getScreenWidth();
        int getScreenHeight();
        virtual void onFrame(int elapsedTime) = 0;
    };

    GameEngine::GameEngine(int screenWidth, int screenHeight, std::string windowTitle, int desiredFrameRate) {
        this->screenWidth = screenWidth;
        this->screenHeight = screenHeight;
        this->frameRate = desiredFrameRate;
        this->timePerFrame = 1.0 / desiredFrameRate;
        window = new sf::RenderWindow(sf::VideoMode(screenWidth, screenHeight), windowTitle);
    }

    void GameEngine::start() {
        while (window->isOpen()) {
            sf::Event event;
            
            while (window->pollEvent(event)) {
                switch (event.type) {
                    case sf::Event::Closed:
                        window->close();
                        delete window;
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

    void GameEngine::renderSprite(Sprite sprite) {
        sf::Texture sfmlTexture;
        sfmlTexture.loadFromFile(sprite.getFilePath());
        sf::Sprite sfmlSprite;
        sfmlSprite.setTexture(sfmlTexture);
        sfmlSprite.setPosition(sprite.getX(), sprite.getY());
        window->draw(sfmlSprite);
    }

    int GameEngine::getScreenWidth() {
        return this->screenWidth;
    }

    int GameEngine::getScreenHeight() {
        return this->screenHeight;
    }

}
