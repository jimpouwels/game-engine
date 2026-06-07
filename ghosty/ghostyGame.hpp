#ifndef ghostyGame_hpp
#define ghostyGame_hpp

#include "gameEngine.hpp"
#include "sprite.hpp"
#include "animatedGraphic.hpp"
#include "vector2D.hpp"
#include "scroller.hpp"
#include "ghostyStageFactory.hpp"
#include "raindrop.hpp"

namespace ghosty {

class GhostyGame : public jimp::GameEngine {

public:
    GhostyGame(int screenWidth, int screenHeight, std::string name) : GameEngine(screenWidth, screenHeight, 5000, name, 1000, false) {
        setBackgroundColor(0x3ba7b3);

        jimp::Sound* rain = createNewSound("/Users/jimpouwels/Projects/game-engine/ghosty/assets/sound/rain.wav");
        jimp::Sound* background = createNewSound("/Users/jimpouwels/Projects/game-engine/ghosty/assets/sound/background.ogg");
        rain->loop(20);
        background->loop(20);
    }

    std::unique_ptr<jimp::StageFactory> getStageFactory() override {
        return std::make_unique<GhostyStageFactory>("/Users/jimpouwels/Projects/game-engine/ghosty/assets/types.json");
    }

    void onUpdate(float elapsedTime) override {
    }

    void onStageLoad(std::string stageFilePath) override {
        for (int i = 0; i < 200; i++) {
            auto raindrop = std::make_unique<Raindrop>();
            if (i % 4 == 0) {
                raindrop->setTransparency(50);
                raindrop->setScale(0.08f);
            }
            registerGraphic(std::move(raindrop));
        }
    }

    void startGame() {
        loadStage("/Users/jimpouwels/Projects/game-engine/ghosty/assets/level1.json");
        start();
    }
};

}

#endif
