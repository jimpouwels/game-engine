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
    Mario(int screenWidth, int screenHeight, std::string name) : GameEngine(screenWidth, screenHeight, 9, name, 600) {
        scrollingWorld = new ScrollingWorld(new Character(jimp::Vector2D { .x = static_cast<float>(getScreenWidth() / 2), .y = 200 }), 100000, 900);
        scrollingWorld->addGraphic(new Platform(jimp::Vector2D { .x = 1200, .y = 200 }));
        scrollingWorld->addGraphic(new Platform(jimp::Vector2D { .x = 1256, .y = 200 }));
        scrollingWorld->addGraphic(new Platform(jimp::Vector2D { .x = 1312, .y = 200 }));
        scrollingWorld->addGraphic(new Platform(jimp::Vector2D { .x = 1600, .y = 260 }));
        scrollingWorld->addGraphic(new Platform(jimp::Vector2D { .x = 1950, .y = 350 }));
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
    mario::Mario game(1500, 900, "My First Platform Game");
    game.startGame();
    return 0;
}
