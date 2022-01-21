#include "gameEngine.hpp"
#include "sprite.hpp"
#include "bullet.hpp"
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
        cleanupBullets();
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
    
    void cleanupBullets() {
        std::list<jimp::AnimatedSprite*> bulletsToRemove;
        for (const auto& bullet: *bullets) {
            if (!isPositionWithinScreen(bullet->getX(), bullet->getY())) {
                bulletsToRemove.push_back(bullet);
            }
        }
        for (const auto& bulletToRemove: bulletsToRemove) {
            bullets->remove(bulletToRemove);
        }
    }
};

int main() {
    Game game(1500, 900, "Fango");
    game.start();
    return 0;
}
