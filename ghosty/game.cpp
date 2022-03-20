#include "gameEngine.hpp"
#include "sprite.hpp"
#include "animatedGraphic.hpp"
#include "gameEngine.hpp"
#include "vector2D.hpp"
#include "scrollingWorld.hpp"
#include "ghostyStageFactory.hpp"
#include "raindrop.hpp"

namespace ghosty {

class GhostyGame : public jimp::GameEngine {
    
private:
    GhostyStageFactory* stageFactory = nullptr;
    
public:
    GhostyGame(int screenWidth, int screenHeight, std::string name) : GameEngine(screenWidth, screenHeight, 5000, name, 1000, false) {
        setBackgroundColor(0x3ba7b3);
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
        
        for (int i = 0; i < 5000; i++) {
            Raindrop* raindrop = new Raindrop();
            if (i % 4 == 0) {
                raindrop->setTransparency(50);
                raindrop->setScale(0.08f);
            }
            registerGraphic(raindrop);
        }
        
        start();
    }
};

}

int main() {
    ghosty::GhostyGame game(1800, 1000, "My First Platform Game");
    game.startGame();
    return 0;
}
