#include "gameEngine.hpp"
#include "sprite.hpp"
#include "asteroid.hpp"
#include "bullet.hpp"
#include "ship.hpp"
#include "gameEngine.hpp"
#include "shipEventListener.hpp"
#include "asteroidSpawner.hpp"
#include <iostream>
#include <chrono>

class Game : public jimp::GameEngine, public ShipEventListener {
    
private:
    jimp::Sprite* background = nullptr;
    Ship* ship = nullptr;
    AsteroidSpawner* asteroidSpawner = nullptr;
    std::list<jimp::AnimatedSprite*>* projectiles = nullptr;
    
public:
    Game(int screenWidth, int screenHeight, std::string name) : GameEngine(screenWidth, screenHeight, name, 60) {
        ship = new Ship(this, this);
        addKeyListener(ship);
        asteroidSpawner = new AsteroidSpawner(this);
        projectiles = new std::list<jimp::AnimatedSprite*>;
        background = new jimp::Sprite(this, 0, 0, 1.0F, "background.jpeg");
    }
    
    ~Game() {
        delete background;
        delete ship;
        delete asteroidSpawner;
        for (const auto& projectile: *projectiles) {
            delete projectile;
        }
        delete projectiles;
    }
    
    void onWeaponFired(jimp::AnimatedSprite* projectile) {
        projectiles->push_back(projectile);
    }
    
    void onFrame(float elapsedTime) {
        draw(background);
        cleanupProjectiles();
        handleProjectileHits();
        for (const auto& projectile: *projectiles) {
            projectile->onFrame(elapsedTime);
        }
        asteroidSpawner->onFrame(elapsedTime);
        ship->onFrame(elapsedTime);
    }
    
    void handleProjectileHits() {
        std::list<jimp::AnimatedSprite*> projectilesToRemove;
        for (const auto& projectile: *projectiles) {
            for (const auto& asteroid: asteroidSpawner->getAsteroids()) {
                if (asteroid->isHitBy(*projectile)) {
                    asteroid->setHit();
                    projectilesToRemove.push_back(projectile);
                    break;
                }
            }
        }
        removeProjectiles(projectilesToRemove);
    }
    
    void cleanupProjectiles() {
        std::list<jimp::AnimatedSprite*> projectilesToRemove;
        for (const auto& projectile: *projectiles) {
            if (!projectile->isPositionedWithinScreen()) {
                projectilesToRemove.push_back(projectile);
            }
        }
        removeProjectiles(projectilesToRemove);
    }
    
    void removeProjectiles(std::list<jimp::AnimatedSprite*>& projectilesToRemove) {
        for (const auto& projectileToRemove: projectilesToRemove) {
            projectiles->remove(projectileToRemove);
            delete projectileToRemove;
        }
    }
};

int main() {
    Game game(1500, 900, "Asteroids");
    game.start();
    return 0;
}
