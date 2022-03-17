#include "gameEngine.hpp"
#include "sprite.hpp"
#include "animatedGraphic.hpp"
#include "gameEngine.hpp"
#include "vector2D.hpp"
#include "scrollingWorld.hpp"
#include "ghostyStageFactory.hpp"

namespace ghosty {

class GhostyGame : public jimp::GameEngine {
    
private:
    GhostyStageFactory* stageFactory = nullptr;
    
public:
    GhostyGame(int screenWidth, int screenHeight, std::string name) : GameEngine(screenWidth, screenHeight, 5000, name, 1000, false) {
        setBackgroundColor(0x3ba7b3);
 
//        jimp::Vector2D flowerPosition = jimp::Vector2D::from(platform->getPosition().x + platform->getWidth() - 40, platform->getPosition().y + platform->getHeight() / 2 - 256);
//        Decoration* flower = new Decoration(flowerPosition, "BlueFlower_{i}.png", 60, 90, 0.5F, 4);
//
//        Decoration* plantSmall = new Decoration(jimp::Vector2D::from(platform->getPosition().x + platform->getWidth() - 130, platform->getPosition().y - 55 ), "plant-small.png", 1, 0, 0.2F, 4);
      
        // SECOND SMALL PLATFORM
//        Platform* platform2 = new Platform("platform-top-left.png", "platform-top-middle.png", 1, 4, 512, 0.3F, jimp::Vector2D::from(platform->getPosition().x + platform->getWidth() + 300, 2100));
//
//        Decoration* plantPoison1 = new Decoration(jimp::Vector2D::from(platform2->getPosition().x + 100, platform2->getPosition().y + 75), "PlantPosion_{i}.png", 30, 180, 0.3F, 4);
//        Decoration* rock = new Decoration(jimp::Vector2D::from(platform2->getPosition().x + 200, platform2->getPosition().y - 110), "rock1.png", 1, 0, 0.25F, 4);
        
        // THIRD SMALL PLATFORM
//        Platform* platform3 = new Platform("platform-top-left.png", "platform-top-middle.png", 1, 2, 512, 0.3F, jimp::Vector2D::from(platform2->getPosition().x + 820, platform2->getPosition().y + platform2->getHeight() + 200 ));
        
        // BACKGROUND
//        Decoration* blackPlant = new Decoration(jimp::Vector2D::from(platform->getPosition().x + platform->getWidth() + 375, platform->getPosition().y + platform->getHeight() - 520), "Plant3_{i}.png", 90, 0, 1.0F, 4);
//        blackPlant->setRgbLevels(jimp::Color::black());
//        blackPlant->animateRgbLevels(jimp::Color::from(90, 90, 90), 1);
//
//        Decoration* rock1 = new Decoration(jimp::Vector2D::from(platform->getPosition().x + 700, platform->getPosition().y - 200), "rock1-background.png", 1, 0, 1.7F, 4);
//        Decoration* spikes = new Decoration(jimp::Vector2D::from(-300, platform->getPosition().y - 300), "spikes1.png", 1, 45, 0.8F, 5);
//        Decoration* hill = new Decoration(jimp::Vector2D::from(-250, platform->getPosition().y + 300), "hill.png", 1, 0, 0.8F, 1);
    }
    
    ~GhostyGame() {
        delete stageFactory;
    }
    
    jimp::StageFactory* getStageFactory() override {
        stageFactory = new GhostyStageFactory("/Users/31098470/Projects/Personal/game-engine/ghosty/assets/types.json");
        return stageFactory;
    }
    
    void startGame() {
        loadStage("/Users/31098470/Projects/Personal/game-engine/ghosty/assets/level1.json");
        start();
    }
};

}

int main() {
    ghosty::GhostyGame game(1800, 1000, "My First Platform Game");
    game.startGame();
    return 0;
}
