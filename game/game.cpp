#include "gameEngine.hpp"
#include "sprite.hpp"
#include "asteroid.hpp"
#include "bullet.hpp"
#include "ship.hpp"
#include "screen.hpp"
#include "shipEventListener.hpp"
#include <iostream>
#include <chrono>

class Game : public jimp::GameEngine, public ShipEventListener {
    
private:
    Ship* ship = nullptr;
    Asteroid* asteroid = nullptr;
    jimp::Screen* screen = nullptr;
    std::list<jimp::AnimatedSprite*>* bullets = nullptr;
    
public:
    Game(int screenWidth, int screenHeight, std::string name) : GameEngine(screenWidth, screenHeight, name, 60) {
        screen = new jimp::Screen(this);
        ship = new Ship(screen, this);
        addKeyListener(ship);
        asteroid = new Asteroid(screen, 10, 10);
        bullets = new std::list<jimp::AnimatedSprite*>;
    }
    
    ~Game() {
        delete ship;
        delete asteroid;
        for (const auto& bullet: *bullets) {
            delete bullet;
        }
        delete bullets;
    }
    
    void onWeaponFired(jimp::AnimatedSprite* projectile) {
        bullets->push_back(projectile);
    }
    
    void onFrame(float elapsedTime) {
        cleanupBullets();
        for (const auto& bullet: *bullets) {
            bullet->update(elapsedTime);
            draw(bullet->getActiveSprite());
        }
        ship->update(elapsedTime);
        asteroid->update(elapsedTime);
        draw(ship->getActiveSprite());
        draw(asteroid->getActiveSprite());
    }
    
    void cleanupBullets() {
        std::list<jimp::AnimatedSprite*> bulletsToRemove;
        for (const auto& bullet: *bullets) {
            if (!bullet->isPositionedWithinScreen()) {
                bulletsToRemove.push_back(bullet);
            }
        }
        for (const auto& bulletToRemove: bulletsToRemove) {
            bullets->remove(bulletToRemove);
            delete bulletToRemove;
        }
    }
};

int main() {
    Game game(1500, 900, "Fango");
    game.start();
    return 0;
}
