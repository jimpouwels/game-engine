#include "gameEngine.hpp"
#include "sprite.hpp"
#include "animatedGraphic.hpp"
#include "gameEngine.hpp"
#include "vector2D.hpp"
#include "character.hpp"
#include "platformBlock.hpp"
#include "decoration.hpp"
#include "scrollingWorld.hpp"
#include "platform.hpp"

namespace ghosty {

class Ghosty : public jimp::GameEngine {
    
private:
    
public:
    Ghosty(int screenWidth, int screenHeight, std::string name) : GameEngine(screenWidth, screenHeight, 9, name, 1000) {
        Character* character = new Character(jimp::Vector2D { .x = static_cast<float>(100), .y = 1400 });
        scrollingWorld = new jimp::ScrollingWorld(character, 10000, 3000);
        setBackgroundColor(0x3ba7b3);

        // LEFT BIG PLATFORM
        Platform* platform = new Platform("platform-top-left.png", "platform-top-middle.png", 5, 7, 512, 0.5F, jimp::Vector2D { .x = -256, .y = static_cast<float>(scrollingWorld->getHeight() - (4 * 256)) });
        Decoration* plant = new Decoration(jimp::Vector2D { .x = 212, .y = platform->getPosition().y - 175 }, "Plant3_{i}.png", 90, 0, 0.5F);
        jimp::Vector2D flowerPosition = jimp::Vector2D { .x = platform->getPosition().x + 7 * 256 - 128, .y = platform->getPosition().y + 200 };
        Decoration* flower = new Decoration(flowerPosition, "BlueFlower_{i}.png", 60, 90, 0.8F);
        Decoration* plantSmall = new Decoration(jimp::Vector2D { .x = 1024, .y = platform->getPosition().y - 75 }, "plant-small.png", 1, 0, 0.3F);
      
        // SECOND SMALL PLATFORM
        Platform* platform2 = new Platform("platform-top-left.png", "platform-top-middle.png", 1, 4, 512, 0.5F, jimp::Vector2D { .x = 1750, .y = 1640 });
        Decoration* plantPoison1 = new Decoration(jimp::Vector2D { .x = platform2->getPosition().x + 100, .y = platform2->getPosition().y + 100 }, "PlantPosion_{i}.png", 30, 180, 0.5F);
        Decoration* rock = new Decoration(jimp::Vector2D { .x = platform2->getPosition().x + 600, .y = platform2->getPosition().y - 100 }, "rock1.png", 1, 0, 0.2F);
        
        // BACKGROUND
        Decoration* rock1 = new Decoration(jimp::Vector2D { .x = platform->getPosition().x + 1500, .y = platform->getPosition().y + 100 }, "rock1-background.png", 1, 0, 1.7F);
        Decoration* spikes = new Decoration(jimp::Vector2D { .x = -600, .y = platform->getPosition().y - 800 }, "spikes1.png", 1, 45, 1.4F);
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
    ghosty::Ghosty game(2800, 1800, "My First Platform Game");
    game.startGame();
    return 0;
}
