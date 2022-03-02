#include "gameEngine.hpp"
#include "sprite.hpp"
#include "animatedGraphic.hpp"
#include "gameEngine.hpp"
#include "vector2D.hpp"
#include "character.hpp"
#include "platform.hpp"
#include "scrollingWorld.hpp"

namespace mario {

class Mario : public jimp::GameEngine {
    
private:
    ScrollingWorld* scrollingWorld = nullptr;
    
public:
    Mario(int screenWidth, int screenHeight, std::string name) : GameEngine(screenWidth, screenHeight, 9, name, 60) {
        scrollingWorld = new ScrollingWorld(new Character(), 50, 1000, 50);
        scrollingWorld->addGraphic(10, 5, new Platform());
        scrollingWorld->addGraphic(30, 5, new Platform());
    }
    
    void startGame() {
        this->start();
    }
    
    void onUpdate(float elapsedTime) {
        scrollingWorld->doOnUpdate();
    }
    
    void onFrame(float elapsedTime) {
        scrollingWorld->doOnFrame();
    }
};

}

int main() {
    mario::Mario game(1500, 900, "Mario");
    game.startGame();
    return 0;
}
