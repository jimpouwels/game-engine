#include "gameEngine.hpp"
#include "sprite.hpp"
#include "animatedGraphic.hpp"
#include "asteroid.hpp"
#include "bullet.hpp"
#include "ship.hpp"
#include "gameEngine.hpp"
#include "asteroidSpawner.hpp"
#include <iostream>
#include <chrono>

class Asteroids : public jimp::GameEngine {
    
private:
    jimp::Sprite* background = nullptr;
    AsteroidSpawner* asteroidSpawner = nullptr;
    jimp::Sound* music = nullptr;
    
public:
    Asteroids(int screenWidth, int screenHeight, std::string name) : GameEngine(screenWidth, screenHeight, 0, name, 60, false) {
        registerGraphic(new Ship());
        music = createNewSound("music.ogg");
        music->loop(25);
        asteroidSpawner = new AsteroidSpawner();
        background = new jimp::Sprite(0, 0, 1.0F, 0, 1, "background.jpeg");
    }
    
    ~Asteroids() {
        delete background;
        delete asteroidSpawner;
    }
    
    void startGame() {
        start();
    }
    
    jimp::StageFactory* getStageFactory() override {
        return nullptr;
    }
    
    void onFrame(float elapsedTime) override {
        draw(background);
    }
    
    void onUpdate(float elapsedTime) override {
        asteroidSpawner->onUpdate(elapsedTime);
    }
};

int main() {
    Asteroids game(1500, 900, "Asteroids");
    game.startGame();
    return 0;
}
