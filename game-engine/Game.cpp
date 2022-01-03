#include "GameEngine.h"

class Game : public jimp::GameEngine {
public:
    Game(int screenWidth, int screenHeight) : GameEngine(screenWidth, screenHeight) {
    }
    
    void onFrame() {
        
    }
};

int main() {
    Game game(1024, 720);
    game.start();
    return 0;
}
