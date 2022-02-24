#include "asteroidSpawner.hpp"
#include "geo2D.hpp"
#include "mathUtils.hpp"

const uint8_t AsteroidSpawner::SECONDS_BETWEEN_ASTEROID_SPAWN = 3;

AsteroidSpawner::AsteroidSpawner() {
}

AsteroidSpawner::~AsteroidSpawner() {
}

void AsteroidSpawner::onUpdate(float elapsedTime) {
    timeSinceLastAsteroid += elapsedTime;
    if (timeSinceLastAsteroid >= SECONDS_BETWEEN_ASTEROID_SPAWN) {
        timeSinceLastAsteroid = 0.0F;

        float x, angle = 0.0F;
        float y = jimp::MathUtils::randomNumberBetween(jimp::GameEngine::getInstance()->getScreenHeight() * 0.25,  jimp::GameEngine::getInstance()->getScreenHeight() * 0.75);
        int randomSide = jimp::MathUtils::randomNumberBetween(0, 2);
        if (randomSide == 0) { // left
            x = -100;
            angle = jimp::MathUtils::randomNumberBetween(45, 135);
        } else { // right
            x = jimp::GameEngine::getInstance()->getScreenWidth() + 100;
            angle = jimp::MathUtils::randomNumberBetween(225, 315);
        }
        new Asteroid(jimp::Vector2D { .x = x, .y = y }, angle);
    }
}
