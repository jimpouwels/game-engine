#include <iostream>
#include <SFML/Graphics.hpp>

namespace jimp {

    class GameEngine {
        
    private:
        int screenWidth;
        int screenHeight;
        
    public:
        GameEngine(int screenWidth, int screenHeight);
        void start();
        virtual void onFrame() = 0;
        
    };

    GameEngine::GameEngine(int screenWidth, int screenHeight) {
        this->screenWidth = screenWidth;
        this->screenHeight = screenHeight;
    }

    void GameEngine::start() {
        sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Test");
        
        while (window.isOpen()) {
            sf::Event event;
            
            while (window.pollEvent(event)) {
                switch (event.type) {
                    case sf::Event::Closed:
                        window.close();
                        break;
                }
            }
            
            onFrame();
            
            window.clear();
            window.display();
        }
    };
}
