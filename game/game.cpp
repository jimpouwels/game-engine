#include "gameEngine.h"
#include "sprite.h"
#include <iostream>
#include <chrono>

class Game : public jimp::GameEngine {
    
public:
    Game(int screenWidth, int screenHeight, std::string name) : GameEngine(screenWidth, screenHeight, name, 60) {
    }
    
    void onFrame(int elapsedTime) {
        renderImage();
    }
    
    void renderImage() {
        renderSprite(jimp::Sprite(50, 50, "fango.png"));
    }
};

int main() {
    Game game(1024, 720, "Fango");
    game.start();
    return 0;
}
