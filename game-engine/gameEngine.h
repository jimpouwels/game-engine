#include <iostream>
#include <SFML/Graphics.hpp>

namespace jimp {

    class GameEngine {
        
    private:
        int screenWidth;
        int screenHeight;
        sf::RenderWindow* window;
        
    public:
        GameEngine(int screenWidth, int screenHeight, std::string windowTitle);
        void start();
        void renderSprite(int x, int y, std::string filePath);
        int getScreenWidth();
        int getScreenHeight();
        virtual void onFrame() = 0;
        
    };

    GameEngine::GameEngine(int screenWidth, int screenHeight, std::string windowTitle) {
        this->screenWidth = screenWidth;
        this->screenHeight = screenHeight;
        window = new sf::RenderWindow(sf::VideoMode(screenWidth, screenHeight), windowTitle);
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
            
            window->clear();
            onFrame();
            window->display();
        }
    }

    void GameEngine::renderSprite(int x, int y, std::string filePath) {
        sf::Texture texture;
        texture.loadFromFile(filePath);
        sf::Sprite sprite;
        sprite.setTexture(texture);
        sprite.setPosition(x, y);
        window->draw(sprite);
    }

    int GameEngine::getScreenWidth() {
        return this->screenWidth;
    }

    int GameEngine::getScreenHeight() {
        return this->screenHeight;
    }

}
