#include "gameEngine.hpp"
#include "sprite.hpp"
#include "fangoCharacter.hpp"
#include "fangoEventListener.hpp"
#include <iostream>
#include <chrono>

class Game : public jimp::GameEngine, public FangoEventListener {
    
private:
    FangoCharacter* fango = nullptr;
    std::list<jimp::AnimatedSprite*>* bullets = nullptr;
    
public:
    Game(int screenWidth, int screenHeight, std::string name) : GameEngine(screenWidth, screenHeight, name, 60) {
        fango = new FangoCharacter(this);
        bullets = new std::list<jimp::AnimatedSprite*>;
        addKeyListener(fango);
    }
    
    ~Game() {
        delete fango;
    }
    
    void onWeaponFired(jimp::AnimatedSprite* projectile) {
        bullets->push_back(projectile);
    }
    
    void onFrame(float elapsedTime) {
        std::cout << "bullet count: " << bullets->size() << std::endl;
        // TODO CLEANUP BULLETS WHEN LEAVING SCREEN
        for (const auto& bullet: *bullets) {
            bullet->update(elapsedTime);
            draw(bullet->getActiveSprite());
        }
        fango->update(elapsedTime);
        renderFango();
    }
    
    void renderFango() {
        draw(fango->getActiveSprite());
    }
};

int main() {
    Game game(640, 480, "Fango");
    game.start();
    return 0;
}
