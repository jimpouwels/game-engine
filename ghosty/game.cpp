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
    GhostyGame(int screenWidth, int screenHeight, std::string name) : GameEngine(screenWidth, screenHeight, 5000, name, 1000, true) {
        setBackgroundColor(0x3ba7b3);

        // THIRD SMALL PLATFORM
//        Platform* platform3 = new Platform("platform-top-left.png", "platform-top-middle.png", 1, 2, 512, 0.3F, jimp::Vector2D::from(platform2->getPosition().x + 820, platform2->getPosition().y + platform2->getHeight() + 200 ));
        
        // BACKGROUND
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
