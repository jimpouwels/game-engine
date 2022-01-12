#include "gameEngine.hpp"
#include "sprite.hpp"
#include "fangoCharacter.hpp"
#include <iostream>
#include <chrono>

class Game : public jimp::GameEngine {
    
private:
    jimp::FangoCharacter* fango;
    
public:
    Game(int screenWidth, int screenHeight, std::string name) : GameEngine(screenWidth, screenHeight, name, 60, 4) {
        fango = new jimp::FangoCharacter();
        addKeyListener(fango);
    }
    
    void onFrame(float elapsedTime) {
        fango->update(elapsedTime);
        renderFango();
    }
    
    void renderFango() {
        draw(fango->getActiveSprite());
    }
};

int main() {
    Game game(640, 480, "Fango");
    game.start();
    return 0;
}
