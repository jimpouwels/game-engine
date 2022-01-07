#include "gameEngine.hpp"
#include "sprite.hpp"
#include "fangoCharacter.hpp"
#include <iostream>
#include <chrono>

class Game : public jimp::GameEngine {
    
private:
    jimp::FangoCharacter* fango;
    
public:
    Game(int screenWidth, int screenHeight, std::string name) : GameEngine(screenWidth, screenHeight, name, 60, 1) {
        this->fango = new jimp::FangoCharacter();
        addKeyListener(fango);
    }
    
    void onFrame(int elapsedTime) {
        renderImage();
    }
    
    void renderImage() {
        draw(*fango);
    }
};

int main() {
    Game game(640, 480, "Fango");
    game.start();
    return 0;
}
