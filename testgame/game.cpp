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
    jimp::Sound* music = nullptr;
    std::list<jimp::AnimatedSprite*>* projectiles = nullptr;
    
public:
    Game(int screenWidth, int screenHeight, std::string name) : GameEngine(screenWidth, screenHeight, name, 60) {
        ship = new Ship(this, this);
        addKeyListener(ship);
        music = new jimp::Sound(this, "music.ogg");
        music->loop(30);
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
        for (const auto& projectile: *projectiles) {
            projectile->onFrame(elapsedTime);
        }
        asteroidSpawner->onFrame(elapsedTime);
        ship->onFrame(elapsedTime);
    }
    
    void onUpdate(float elapsedTime) {
        cleanupProjectilesOutsideScreen();
        handleAsteroidsHitByAsteroids();
        handleProjectileCollisionWithAsteroids();
        handleShipCollisionWithAsteroids();
        for (const auto& projectile: *projectiles) {
            projectile->onUpdate(elapsedTime);
        }
        asteroidSpawner->onUpdate(elapsedTime);
        ship->onUpdate(elapsedTime);
    }
    
    void handleProjectileCollisionWithAsteroids() {
        std::list<jimp::AnimatedSprite*> projectilesToRemove;
        std::list<jimp::AnimatedSprite*> asteroidsToRemove;
        for (const auto& projectile: *projectiles) {
            for (const auto& asteroid: asteroidSpawner->getAsteroids()) {
                projectile->checkCollisionRect(asteroid);
                if (projectile->isMarkedForDeletion()) {
                    projectilesToRemove.push_back(projectile);
                    break;
                }
            }
        }
        removeProjectiles(projectilesToRemove);
    }
    
    void handleAsteroidsHitByAsteroids() {
        for (uint16_t i = 0; i < asteroidSpawner->getAsteroids().size(); i++) {
            for (uint16_t j = i + 1; j < asteroidSpawner->getAsteroids().size(); j++) {
                asteroidSpawner->getAsteroids().at(i)->checkCollisionRect(asteroidSpawner->getAsteroids().at(j));
            }
        }
    }
    
    void handleShipCollisionWithAsteroids() {
        for (const auto& asteroid: asteroidSpawner->getAsteroids()) {
            asteroid->checkCollisionRect(ship);
        }
    }
    
    void cleanupProjectilesOutsideScreen() {
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
