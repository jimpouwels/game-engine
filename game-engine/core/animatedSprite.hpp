#ifndef animatedCharacter_hpp
#define animatedCharacter_hpp

#include <stdio.h>
#include <vector>
#include <map>
#include "sprite.hpp"
#include "animation.hpp"
#include "gameEngine.hpp"
#include "vector2D.hpp"
#include "geo2D.hpp"

namespace jimp {

class AnimatedSprite {

private:
    Vector2D position = { .x = 0, .y = 0 };
    float scale;
    float angle;
    GameEngine* gameEngine;
    std::map<std::string, Animation*>* animationMap;
    Animation* activeAnimation = nullptr;
    bool markedForDeletion = false;
    bool isAccelerating = false;
    float velocityAngle = 0;
    bool updateCurrentVelocity = false;
    uint16_t mass = 0;
    uint16_t moveForce = 0;
    Vector2D velocity = Vector2D { .x = 0, .y = 0 };
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
    void markForDeletion() {
        markedForDeletion = true;
    }
    void accelerate(float angle, uint16_t mass, uint16_t force);
    void updateVelocityAngle(float angle);
    virtual void hasCollidedRectLeft(AnimatedSprite* otherSprite) {};
    virtual void hasCollidedRectRight(AnimatedSprite* otherSprite) {};
    virtual void hasCollidedRectTop(AnimatedSprite* otherSprite) {};
    virtual void hasCollidedRectBottom(AnimatedSprite* otherSprite) {};
    virtual void hasCollidedRect(AnimatedSprite* otherSprite, Geo2D::Side side) {};
    
public:
    AnimatedSprite(GameEngine* gameEngine, float x, float y, float scale, int rotationAngle, float imageSwapIntervalInSeconds);
    AnimatedSprite(GameEngine* gameEngine, float x, float y, float scale, float imageSwapIntervalInSeconds);
    ~AnimatedSprite();
    GameEngine* getGameEngine();
    Vector2D& getPosition();
    Vector2D& getVelocity();
    float getScale();
    int getWidth();
    int getHeight();
    float getRotationAngle();
    void setRotationAngle(float angle);
    virtual bool isPositionedWithinScreen();
    bool isAtBottomEdgeOfScreen();
    bool isAtTopEdgeOfScreen();
    bool isAtLeftEdgeOfScreen();
    bool isAtRightEdgeOfScreen();
    bool isOutsideScreenBottom();
    bool isOutsideScreenTop();
    bool isOutsideScreenLeft();
    bool isOutsideScreenRight();
    bool isMarkedForDeletion();
    bool checkCollisionRect(AnimatedSprite* otherSprite);
    uint16_t getVelocityAngle();
    Sprite* getActiveSprite();
    virtual void onFrame(float elapsedTime) {
        gameEngine->draw(getActiveSprite());
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
