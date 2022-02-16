#ifndef animatedCharacter_hpp
#define animatedCharacter_hpp

#include <stdio.h>
#include <vector>
#include <map>
#include "sprite.hpp"
#include "animation.hpp"
#include "vector2D.hpp"
#include "geo2D.hpp"

namespace jimp {

class AnimatedSprite {
    
private:
    Vector2D position = { .x = 0, .y = 0 };
    float scale;
    float angle;
    std::map<std::string, Animation*>* animationMap;
    Animation* activeAnimation = nullptr;
    bool markedForDeletion = false;
    bool isAccelerating = false;
    float velocityAngle = 0;
    bool updateCurrentVelocity = false;
    uint16_t mass = 0;
    uint16_t moveForce = 0;
    Vector2D velocity = Vector2D { .x = 0, .y = 0 };
    std::mutex* lock = new std::mutex();
    float elapsedTimeSinceLastSwap;
    float imageSwapIntervalInSeconds;
    void updateCurrentSpriteData();
    void updateAnimation(float elapsedTime);
    void updateMovement(float elapsedTime);
    void draw(float elapsedTime);
    
protected:
    void setX(float x);
    void setY(float y);
    void setPosition(Vector2D position);
    void addToPosition(Vector2D delta);
    void setCurrentAnimation(std::string animationId);
    void accelerate(float angle, uint16_t mass, uint16_t force);
    virtual void hasCollidedRectLeft(AnimatedSprite* otherSprite) {};
    virtual void hasCollidedRectRight(AnimatedSprite* otherSprite) {};
    virtual void hasCollidedRectTop(AnimatedSprite* otherSprite) {};
    virtual void hasCollidedRectBottom(AnimatedSprite* otherSprite) {};
    virtual void hasCollidedRect(AnimatedSprite* otherSprite, Geo2D::Side side) {};
    
public:
    AnimatedSprite( float x, float y, float scale, int rotationAngle, float imageSwapIntervalInSeconds);
    AnimatedSprite( float x, float y, float scale, float imageSwapIntervalInSeconds);
    ~AnimatedSprite();
    Vector2D& getPosition();
    Vector2D& getVelocity();
    float getScale();
    int getWidth();
    int getHeight();
    float getRotationAngle();
    void setRotationAngle(float angle);
    bool isMarkedForDeletion();
    bool checkCollisionRect(AnimatedSprite* otherSprite);
    uint16_t getVelocityAngle();
    Sprite* getActiveSprite();
    std::list<Sprite*> getAllSprites();
    void tryLock();
    void unlock();
    void markForDeletion() {
        markedForDeletion = true;
    }
    virtual void onFrame(float elapsedTime) {
    };
    virtual void onUpdate(float elapsedTime) {
        updateMovement(elapsedTime);
        this->updateAnimation(elapsedTime);
    }
    virtual Vector2D getRotationPoint() {
        return Vector2D { getWidth() / 2.0F, .y = getHeight() / 2.0F };
    }
    void addSprite(std::string animationId, std::string filePath);
};
}

#endif
