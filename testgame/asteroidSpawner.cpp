#include "asteroidSpawner.hpp"
#include "geo2D.hpp"
#include "mathUtils.hpp"

const int AsteroidSpawner::SECONDS_BETWEEN_ASTEROID_SPAWN = 3;

AsteroidSpawner::AsteroidSpawner(jimp::GameEngine* gameEngine) {
    asteroids = new std::list<Asteroid*>;
    this->gameEngine = gameEngine;
}

AsteroidSpawner::~AsteroidSpawner() {
    for (const auto& asteroid: *asteroids) {
        delete asteroid;
    }
    delete asteroids;
}

void AsteroidSpawner::onFrame(float elapsedTime) {
    timeSinceLastAsteroid += elapsedTime;
    if (timeSinceLastAsteroid >= SECONDS_BETWEEN_ASTEROID_SPAWN) {
        timeSinceLastAsteroid = 0.0F;
        
//        float x, y, angle = 0.0F;
//        int randomSide = jimp::MathUtils::randomNumberBetween(0, 2);
//        if (randomSide == 0) { // left
//            x = -100;
//            y = jimp::MathUtils::randomNumberBetween(screen->getHeight() * 0.25, screen->getHeight() * 0.75);
//            angle = jimp::MathUtils::randomNumberBetween(45, 135);
//        } else { // right
//            x = screen->getWidth() + 100;
//            y = jimp::MathUtils::randomNumberBetween(screen->getHeight() * 0.25, screen->getHeight() * 0.75);
//            angle = jimp::MathUtils::randomNumberBetween(225, 315);
//        }
        asteroids->push_back(new Asteroid(gameEngine, 10, 10, 135));
    }
    std::list<Asteroid*> asteroidsToDelete;
    for (const auto& asteroid: *asteroids) {
        if (asteroid->isDestroyed()) {
            asteroidsToDelete.push_back(asteroid);
        } else {
            asteroid->onFrame(elapsedTime);
        }
    }
    deleteAsteroids(asteroidsToDelete);
}

std::list<Asteroid*>& AsteroidSpawner::getAsteroids() {
    return *asteroids;
}

void AsteroidSpawner::deleteAsteroids(std::list<Asteroid*>& asteroidsToDelete) {
    for (const auto& asteroid: asteroidsToDelete) {
        asteroids->remove(asteroid);
        delete asteroid;
    }
}
