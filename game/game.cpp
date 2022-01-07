#include "gameEngine.hpp"
#include "sprite.hpp"
#include "animatedCharacter.hpp"
#include <iostream>
#include <chrono>

class Game : public jimp::GameEngine {
    
private:
    jimp::AnimatedCharacter* fango;
    
public:
    Game(int screenWidth, int screenHeight, std::string name) : GameEngine(screenWidth, screenHeight, name, 60, 4) {
        this->fango = new jimp::AnimatedCharacter(new jimp::Sprite(0, 0, "fango.png"));
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
