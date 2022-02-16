#include "gameEngine.hpp"
#include "sprite.hpp"
#include "animatedSprite.hpp"
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
    AsteroidSpawner* asteroidSpawner = nullptr;
    jimp::Sound* music = nullptr;
    std::list<jimp::AnimatedSprite*>* projectiles = nullptr;
    
public:
    Game(int screenWidth, int screenHeight, std::string name) : GameEngine(screenWidth, screenHeight, name, 60) {
        registerAnimatedSprite(new Ship(this, this));
        music = new jimp::Sound("music.ogg");
        registerSound(music);
        music->loop(25);
        asteroidSpawner = new AsteroidSpawner(this);
        projectiles = new std::list<jimp::AnimatedSprite*>;
        background = new jimp::Sprite(0, 0, 1.0F, "background.jpeg");
    }
    
    ~Game() {
        delete background;
        delete asteroidSpawner;
        delete projectiles;
    }
    
    void onWeaponFired(jimp::AnimatedSprite* projectile) {
        projectiles->push_back(projectile);
    }
    
    void onFrame(float elapsedTime) {
        draw(background);
        asteroidSpawner->onFrame(elapsedTime);
    }
    
    void onUpdate(float elapsedTime) {
        cleanupProjectilesOutsideScreen();
        asteroidSpawner->onUpdate(elapsedTime);
    }
    
    void onSpriteDeleted(jimp::AnimatedSprite* animatedSprite) {
        Asteroid* asteroid = dynamic_cast<Asteroid*>(animatedSprite);
        if (asteroid != nullptr) {
            asteroidSpawner->deleteAsteroid(asteroid);
        }
        Bullet* projectile = dynamic_cast<Bullet*>(animatedSprite);
        if (projectile != nullptr) {
            projectiles->remove(projectile);
        }
    }
    
    void cleanupProjectilesOutsideScreen() {
        std::list<jimp::AnimatedSprite*> projectilesToRemove;
        for (const auto& projectile: *projectiles) {
            if (!isPositionWithinScreen(projectile->getPosition())) {
                projectilesToRemove.push_back(projectile);
            }
        }
        removeProjectiles(projectilesToRemove);
    }
    
    void removeProjectiles(std::list<jimp::AnimatedSprite*>& projectilesToRemove) {
        for (const auto& projectileToRemove: projectilesToRemove) {
            projectiles->remove(projectileToRemove);
            projectileToRemove->markForDeletion();
        }
    }
};

int main() {
    Game game(1500, 900, "Asteroids");
    game.start();
    return 0;
}
