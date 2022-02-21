#ifndef animatedCharacter_hpp
#define animatedCharacter_hpp

#include <stdio.h>
#include <vector>
#include <map>
#include "sprite.hpp"
#include "animation.hpp"
#include "vector2D.hpp"
#include "geo2D.hpp"
#include "keyListener.hpp"

namespace jimp {

class AnimatedSprite : public KeyListener {
    
private:
    Vector2D position = { .x = 0, .y = 0 };
    float scale;
    float angle;
    std::map<std::string, Animation*>* animationMap;
    Animation* activeAnimation = nullptr;
    bool markedForDeletion = false;
    bool deleteOnLeaveScreen = false;
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
    void setCurrentAnimation(std::string animationId);
    void accelerate(float angle, uint16_t mass, uint16_t force);
    virtual void doOnUpdate(float elapsedTime) { };
    virtual void doOnFrame(float elapsedTime) { };
    virtual void hasCollidedRectLeft(AnimatedSprite* otherSprite) {};
    virtual void hasCollidedRectRight(AnimatedSprite* otherSprite) {};
    virtual void hasCollidedRectTop(AnimatedSprite* otherSprite) {};
    virtual void hasCollidedRectBottom(AnimatedSprite* otherSprite) {};
    virtual void hasCollidedRect(AnimatedSprite* otherSprite, Geo2D::Side side) {};
    
public:
    AnimatedSprite(Vector2D position, float scale, int rotationAngle, float imageSwapIntervalInSeconds);
    AnimatedSprite(Vector2D position, float scale, float imageSwapIntervalInSeconds);
    ~AnimatedSprite();
    Vector2D& getPosition();
    Vector2D& getVelocity();
    float getScale();
    int getWidth();
    int getHeight();
    float getRotationAngle();
    void setRotationAngle(float angle);
    void setDeleteOnLeaveScreen(bool deleteOnLeaveScreen);
    bool isMarkedForDeletion();
    bool checkCollisionRect(AnimatedSprite* otherSprite);
    uint16_t getVelocityAngle();
    Sprite* getActiveSprite();
    std::list<Sprite*> getAllSprites();
    void markForDeletion();
    void setX(float x);
    void setY(float y);
    void setPosition(Vector2D position);
    void addToPosition(Vector2D delta);
    void onFrame(float elapsedTime);
    void onUpdate(float elapsedTime);
    virtual Vector2D getRotationPoint();
    virtual uint16_t getZIndex() {
        return 0;
    }
    void addSprite(std::string animationId, std::string filePath);
    void onKeyboardLeft(jimp::KeyState keyState) {};
    void onKeyboardRight(jimp::KeyState keyState) {};
    void onKeyboardUp(jimp::KeyState keyState) {};
    void onKeyboardSpaceBar(jimp::KeyState keyState) {};
};
}

#endif
