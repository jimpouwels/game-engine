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
    Asteroids(int screenWidth, int screenHeight, std::string name) : GameEngine(screenWidth, screenHeight, name, 60) {
        new Ship();
        music = new jimp::Sound("music.ogg");
        registerSound(music);
        music->loop(25);
        asteroidSpawner = new AsteroidSpawner();
        background = new jimp::Sprite(0, 0, 1.0F, "background.jpeg");
    }
    
    ~Asteroids() {
        delete background;
        delete asteroidSpawner;
    }
    
    void startGame() {
        this->start();
    }
    
    void onFrame(float elapsedTime) {
        draw(background);
    }
    
    void onUpdate(float elapsedTime) {
        asteroidSpawner->onUpdate(elapsedTime);
    }
};

//int main() {
//    Asteroids game(1500, 900, "Asteroids");
//    game.startGame();
//    return 0;
//}
