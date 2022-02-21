#ifndef asteroidSpawner_hpp
#define asteroidSpawner_hpp

#include <list>
#include "asteroid.hpp"

class AsteroidSpawner {
    
private:
    static const uint8_t SECONDS_BETWEEN_ASTEROID_SPAWN;
    float timeSinceLastAsteroid = 0.0F;
    
public:
    AsteroidSpawner();
    ~AsteroidSpawner();
    void onUpdate(float elapsedTime);
    
};

#endif
