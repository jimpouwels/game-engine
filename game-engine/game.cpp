#include "gameEngine.h"
#include <iostream>

class Game : public jimp::GameEngine {
public:
    Game(int screenWidth, int screenHeight, std::string name) : GameEngine(screenWidth, screenHeight, name) {
    }
    
    void onFrame() {
        renderImage();
    }
    
    void renderImage() {
        renderSprite(50, 50, "fango.png");
    }
};

int main() {
    Game game(1024, 720, "Fango");
    game.start();
    return 0;
}
