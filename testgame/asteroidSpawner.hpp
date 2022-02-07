#ifndef asteroidSpawner_hpp
#define asteroidSpawner_hpp

#include <list>
#include "asteroid.hpp"

class AsteroidSpawner {
    
private:
    static const int SECONDS_BETWEEN_ASTEROID_SPAWN;
    jimp::GameEngine* gameEngine = nullptr;
    std::list<Asteroid*>* asteroids = nullptr;
    float timeSinceLastAsteroid = 0.0F;
    void deleteAsteroids(std::list<Asteroid*>& asteroidsToDelete);
    
public:
    AsteroidSpawner(jimp::GameEngine* gameEngine);
    ~AsteroidSpawner();
    void onFrame(float elapsedTime);
    std::list<Asteroid*>& getAsteroids();
    
};

#endif
