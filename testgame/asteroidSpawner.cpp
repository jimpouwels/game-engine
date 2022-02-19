#include "asteroidSpawner.hpp"
#include "geo2D.hpp"
#include "mathUtils.hpp"

const uint8_t AsteroidSpawner::SECONDS_BETWEEN_ASTEROID_SPAWN = 3;

AsteroidSpawner::AsteroidSpawner(jimp::GameEngine* gameEngine) {
    this->gameEngine = gameEngine;
}

AsteroidSpawner::~AsteroidSpawner() {
}

void AsteroidSpawner::onUpdate(float elapsedTime) {
    timeSinceLastAsteroid += elapsedTime;
    if (timeSinceLastAsteroid >= SECONDS_BETWEEN_ASTEROID_SPAWN) {
        timeSinceLastAsteroid = 0.0F;

        float x, angle = 0.0F;
        float y = jimp::MathUtils::randomNumberBetween(gameEngine->getScreenHeight() * 0.25, gameEngine->getScreenHeight() * 0.75);
        int randomSide = jimp::MathUtils::randomNumberBetween(0, 2);
        if (randomSide == 0) { // left
            x = -100;
            angle = jimp::MathUtils::randomNumberBetween(45, 135);
        } else { // right
            x = gameEngine->getScreenWidth() + 100;
            angle = jimp::MathUtils::randomNumberBetween(225, 315);
        }
        Asteroid *newAsteroid = new Asteroid(gameEngine, jimp::Vector2D { .x = x, .y = y }, angle);
        gameEngine->registerAnimatedSprite(newAsteroid);
    }
}
