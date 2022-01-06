#include "gameEngine.h"
#include "sprite.h"
#include "animatedCharacter.h"
#include <iostream>
#include <chrono>

class Game : public jimp::GameEngine {
    
public:
    Game(int screenWidth, int screenHeight, std::string name) : GameEngine(screenWidth, screenHeight, name, 60, 4) {
    }
    
    void onFrame(int elapsedTime) {
        renderImage();
    }
    
    void renderImage() {
        draw(jimp::AnimatedCharacter(new jimp::Sprite(0, 00, "fango.png")));
    }
};

int main() {
    Game game(640, 480, "Fango");
    game.start();
    return 0;
}
