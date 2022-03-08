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
        scrollingWorld = new jimp::ScrollingWorld(character, 5000, 3000);
        setBackgroundColor(0x79b5af);

        Platform* platform = new Platform("platform-top-left.png", "platform-top-middle.png", 5, 10, 512, 0.5F, jimp::Vector2D { .x = -256, .y = static_cast<float>(scrollingWorld->getHeight() - (4 * 256)) });
        
        Decoration* plant = new Decoration(jimp::Vector2D { .x = 512, .y = platform->getPosition().y - 175 }, "Plant3_{i}.png", 90, 0);
  
        jimp::Vector2D flowerPosition = jimp::Vector2D { .x = platform->getPosition().x + 7 * 256 - 128, .y = platform->getPosition().y + 400 };
        Decoration* flower = new Decoration(flowerPosition, "BlueFlower_{i}.png", 60, 90);
      
        
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
