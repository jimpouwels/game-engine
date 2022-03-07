#include "gameEngine.hpp"
#include "sprite.hpp"
#include "animatedGraphic.hpp"
#include "gameEngine.hpp"
#include "vector2D.hpp"
#include "character.hpp"
#include "platform.hpp"
#include "floor.hpp"
#include "plant.hpp"
#include "scrollingWorld.hpp"

namespace mario {

class Mario : public jimp::GameEngine {
    
private:
    
public:
    Mario(int screenWidth, int screenHeight, std::string name) : GameEngine(screenWidth, screenHeight, 9, name, 1000) {
        Character* character = new Character(jimp::Vector2D { .x = static_cast<float>(100), .y = 2700 });
        scrollingWorld = new jimp::ScrollingWorld(character, 5000, 3000);
        setBackgroundColor(0x79b5af);

        Floor* left = new Floor(jimp::Vector2D { .x = 0, .y = 0 }, "platform-top-left.png", 1);
        Floor* middle = new Floor(jimp::Vector2D { .x = static_cast<float>(left->getWidth()), .y = 0 }, "platform-top-middle.png", 3);
        Floor* right = new Floor(jimp::Vector2D { .x = (middle->getPosition().x + middle->getWidth()), .y = 0 }, "platform-top-right.png", 1);
        
        Plant* plant = new Plant(jimp::Vector2D { .x = middle->getPosition().x + (middle->getSingleWidth() * 2), .y = middle->getPosition().y - 128 }, 1);
        
//        new Platform(jimp::Vector2D { .x = 1200, .y = 2590 }, 5);
//        new Platform(jimp::Vector2D { .x = 1600, .y = 2430 }, 8);
//        new Platform(jimp::Vector2D { .x = 1800, .y = 2230 }, 2);
//        new Platform(jimp::Vector2D { .x = 2000, .y = 2030 }, 2);
//        new Platform(jimp::Vector2D { .x = 2200, .y = 1830 }, 2);
//        new Platform(jimp::Vector2D { .x = 2400, .y = 1630 }, 2);
//        new Platform(jimp::Vector2D { .x = 2600, .y = 1430 }, 2);
//        new Platform(jimp::Vector2D { .x = 2800, .y = 1230 }, 2);
//        new Platform(jimp::Vector2D { .x = 3000, .y = 1030 }, 2);
//        new Platform(jimp::Vector2D { .x = 3200, .y = 830 }, 2);
//        new Platform(jimp::Vector2D { .x = 3400, .y = 630 }, 2);
//        new Platform(jimp::Vector2D { .x = 3600, .y = 430 }, 2);
//        new Platform(jimp::Vector2D { .x = 3800, .y = 230 }, 2);
        
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
    mario::Mario game(2800, 1800, "My First Platform Game");
    game.startGame();
    return 0;
}
