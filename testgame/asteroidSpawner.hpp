#ifndef asteroidSpawner_hpp
#define asteroidSpawner_hpp

#include <list>
#include "asteroid.hpp"

class AsteroidSpawner {
    
private:
    static const uint8_t SECONDS_BETWEEN_ASTEROID_SPAWN;
    jimp::GameEngine* gameEngine = nullptr;
    float timeSinceLastAsteroid = 0.0F;
    
public:
    AsteroidSpawner(jimp::GameEngine* gameEngine);
    ~AsteroidSpawner();
    void onUpdate(float elapsedTime);
    
};

#endif
