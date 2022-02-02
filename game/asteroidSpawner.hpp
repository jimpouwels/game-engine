#ifndef asteroidSpawner_hpp
#define asteroidSpawner_hpp

#include <list>
#include "asteroid.hpp"
#include "screen.hpp"

class AsteroidSpawner {
    
private:
    static const int SECONDS_BETWEEN_ASTEROID_SPAWN;
    jimp::Screen* screen = nullptr;
    std::list<Asteroid*>* asteroids = nullptr;
    float timeSinceLastAsteroid = 0.0F;
    
public:
    AsteroidSpawner(jimp::Screen* screen);
    ~AsteroidSpawner();
    void onFrame(float elapsedTime);
    std::list<Asteroid*>& getAsteroids();
    
};

#endif
