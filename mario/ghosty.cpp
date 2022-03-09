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
    Ghosty(int screenWidth, int screenHeight, std::string name) : GameEngine(screenWidth, screenHeight, 18, name, 1000) {
        Character* character = new Character(jimp::Vector2D { .x = static_cast<float>(100), .y = 1400 });
        scrollingWorld = new jimp::ScrollingWorld(character, 10000, 3000);
        setBackgroundColor(0x3ba7b3);

        // LEFT BIG PLATFORM
        Platform* platform = new Platform("platform-top-left.png", "platform-top-middle.png", 5, 7, 512, 0.3F, jimp::Vector2D { .x = -153, .y = static_cast<float>(scrollingWorld->getHeight() - (4 * 153)) });
        Decoration* plant = new Decoration(jimp::Vector2D { .x = platform->getPosition().x + 200, .y = platform->getPosition().y - 95 }, "Plant3_{i}.png", 90, 0, 0.3F);
        
        jimp::Vector2D flowerPosition = jimp::Vector2D { .x = platform->getPosition().x + platform->getWidth() - 40, .y = platform->getPosition().y + platform->getHeight() / 2 - 256 };
        Decoration* flower = new Decoration(flowerPosition, "BlueFlower_{i}.png", 60, 90, 0.5F);
        
        Decoration* plantSmall = new Decoration(jimp::Vector2D { .x = platform->getPosition().x + platform->getWidth() - 130, .y = platform->getPosition().y - 55 }, "plant-small.png", 1, 0, 0.2F);
      
        // SECOND SMALL PLATFORM
        Platform* platform2 = new Platform("platform-top-left.png", "platform-top-middle.png", 1, 4, 512, 0.3F, jimp::Vector2D { .x = platform->getPosition().x + platform->getWidth() + 300, .y = 2100 });
        Decoration* plantPoison1 = new Decoration(jimp::Vector2D { .x = platform2->getPosition().x + 100, .y = platform2->getPosition().y + 75 }, "PlantPosion_{i}.png", 30, 180, 0.3F);
        Decoration* rock = new Decoration(jimp::Vector2D { .x = platform2->getPosition().x + 200, .y = platform2->getPosition().y - 110 }, "rock1.png", 1, 0, 0.25F);
        
        // THIRD SMALL PLATFORM
        Platform* platform3 = new Platform("platform-top-left.png", "platform-top-middle.png", 1, 2, 512, 0.3F, jimp::Vector2D { .x = platform2->getPosition().x + 820, .y = platform2->getPosition().y + platform2->getHeight() + 200 });
        
        // BACKGROUND
        Decoration* blackPlant = new Decoration(jimp::Vector2D { .x = platform->getPosition().x + platform->getWidth() + 375, .y = platform->getPosition().y + platform->getHeight() - 520 }, "Plant3_{i}.png", 90, 0, 1.0F);
        blackPlant->setRgbLevels(jimp::Color::black());
        blackPlant->animateRgbLevels(jimp::Color::custom(90, 90, 90), 1);
        
        Decoration* rock1 = new Decoration(jimp::Vector2D { .x = platform->getPosition().x + 700, .y = platform->getPosition().y - 200 }, "rock1-background.png", 1, 0, 1.7F);
        Decoration* spikes = new Decoration(jimp::Vector2D { .x = -300, .y = platform->getPosition().y - 300 }, "spikes1.png", 1, 45, 0.8F);
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
    ghosty::Ghosty game(2000, 1200, "My First Platform Game");
    game.startGame();
    return 0;
}
