#include "asteroidSpawner.hpp"

const int AsteroidSpawner::SECONDS_BETWEEN_ASTEROID_SPAWN = 5;

AsteroidSpawner::AsteroidSpawner(jimp::Screen* screen) {
    asteroids = new std::list<Asteroid*>;
    this->screen = screen;
}

AsteroidSpawner::~AsteroidSpawner() {
    for (const auto& asteroid: *asteroids) {
        delete asteroid;
    }
}

void AsteroidSpawner::onFrame(float elapsedTime) {
    timeSinceLastAsteroid += elapsedTime;
    if (timeSinceLastAsteroid >= SECONDS_BETWEEN_ASTEROID_SPAWN) {
        timeSinceLastAsteroid = 0.0F;
        asteroids->push_back(new Asteroid(screen, 10, 10));
    }
    for (const auto& asteroid: *asteroids) {
        asteroid->update(elapsedTime);
    }
}

std::list<Asteroid*>& AsteroidSpawner::getAsteroids() {
    return *asteroids;
}
