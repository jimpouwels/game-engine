#include "gameEngine.hpp"
#include "sprite.hpp"
#include "animatedGraphic.hpp"
#include "gameEngine.hpp"
#include "vector2D.hpp"
#include "character.hpp"
#include "platformBlock.hpp"
#include "decoration.hpp"
#include "scrollingWorld.hpp"

namespace ghosty {

class Ghosty : public jimp::GameEngine {
    
private:
    
public:
    Ghosty(int screenWidth, int screenHeight, std::string name) : GameEngine(screenWidth, screenHeight, 9, name, 1000) {
        Character* character = new Character(jimp::Vector2D { .x = static_cast<float>(100), .y = 2700 });
        scrollingWorld = new jimp::ScrollingWorld(character, 5000, 3000);
        setBackgroundColor(0x79b5af);

        PlatformBlock* left = new PlatformBlock(jimp::Vector2D { .x = 0, .y = 0 }, "platform-top-left.png", 1);
        PlatformBlock* middle = new PlatformBlock(jimp::Vector2D { .x = static_cast<float>(left->getWidth()), .y = 0 }, "platform-top-middle.png", 3);
        PlatformBlock* right = new PlatformBlock(jimp::Vector2D { .x = (middle->getPosition().x + middle->getWidth()), .y = 0 }, "platform-top-right.png", 1);
        
        Decoration* plant = new Decoration(jimp::Vector2D { .x = middle->getPosition().x + (middle->getSingleWidth() * 2), .y = middle->getPosition().y - 175 }, "Plant3_{i}.png", 90, 0);
        
        jimp::Vector2D flowerPosition = jimp::Vector2D { .x = right->getPosition().x, .y = right->getPosition().y };
        flowerPosition.x += 128;
        flowerPosition.y -= 128;
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
