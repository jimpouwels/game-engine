#include "gameEngine.hpp"
#include "sprite.hpp"
#include "animatedGraphic.hpp"
#include "gameEngine.hpp"
#include "vector2D.hpp"
#include "character.hpp"
#include "platform.hpp"
#include "floor.hpp"
#include "scrollingWorld.hpp"
#include "backgroundRedCharacter.hpp"

namespace mario {

class Mario : public jimp::GameEngine {
    
private:
    
public:
    Mario(int screenWidth, int screenHeight, std::string name) : GameEngine(screenWidth, screenHeight, 9, name, 1000) {
        scrollingWorld = new jimp::ScrollingWorld(new Character(jimp::Vector2D { .x = static_cast<float>(getScreenWidth() / 2), .y = 200 }), 5000, 3000);
        setBackgroundColor(0x7fb4c9);
        new BackgroundRedCharacter(jimp::Vector2D { .x = 1670, .y = 2600 });
        new Floor(jimp::Vector2D { .x = 0, .y = 0 }, 500);
        
        new Platform(jimp::Vector2D { .x = 1200, .y = 2590 }, 5);
        new Platform(jimp::Vector2D { .x = 1600, .y = 2430 }, 8);
        new Platform(jimp::Vector2D { .x = 1800, .y = 2230 }, 2);
        new Platform(jimp::Vector2D { .x = 2000, .y = 2030 }, 2);
        new Platform(jimp::Vector2D { .x = 2200, .y = 1830 }, 2);
        new Platform(jimp::Vector2D { .x = 2400, .y = 1630 }, 2);
        new Platform(jimp::Vector2D { .x = 2600, .y = 1430 }, 2);
        new Platform(jimp::Vector2D { .x = 2800, .y = 1230 }, 2);
        new Platform(jimp::Vector2D { .x = 3000, .y = 1030 }, 2);
        new Platform(jimp::Vector2D { .x = 3200, .y = 830 }, 2);
        new Platform(jimp::Vector2D { .x = 3400, .y = 630 }, 2);
        new Platform(jimp::Vector2D { .x = 3600, .y = 430 }, 2);
        new Platform(jimp::Vector2D { .x = 3800, .y = 230 }, 2);
        
    }
    
    void startGame() {
        this->start();
    }
    
    void onUpdate(float elapsedTime) {
    }
    
    void onFrame(float elapsedTime) {
    }
};

}

int main() {
    mario::Mario game(1500, 900, "My First Platform Game");
    game.startGame();
    return 0;
}
