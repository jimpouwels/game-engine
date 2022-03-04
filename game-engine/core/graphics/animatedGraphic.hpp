#ifndef animatedCharacter_hpp
#define animatedCharacter_hpp

#include <stdio.h>
#include <vector>
#include <map>
#include "drawable.hpp"
#include "animation.hpp"
#include "vector2D.hpp"
#include "geo2D.hpp"
#include "keyListener.hpp"
#include "shape.hpp"

namespace jimp {

class AnimatedGraphic : public KeyListener {
    
private:
    Vector2D position = { .x = 0, .y = 0 };
    float scale;
    float angle;
    bool visible = true;
    bool applyGravity = false;
    std::map<std::string, Animation*>* animationMap;
    Animation* activeAnimation = nullptr;
    bool markedForDeletion = false;
    bool deleteOnLeaveScreen = false;
    Vector2D moveVelocity = Vector2D { .x = 0, .y = 0 };
    Vector2D gravityVelocity = Vector2D { .x = 0, .y = 0 };
    bool gravityBlocked = false;
    bool moving = false;
    float moveForce = 0;
    float moveAngle = 0;
    std::mutex* lock = new std::mutex();
    float elapsedTimeSinceLastSwap;
    float drawableSwapIntervalInSeconds;
    void updateCurrentDrawableData();
    void updateAnimation(float elapsedTime);
    void updateMovement(float elapsedTime);
    void draw(float elapsedTime);
    void addDrawable(std::string animationId, Drawable* drawable);
    
protected:
    void setCurrentAnimation(std::string animationId);
    void accelerate(float angle, uint16_t mass, uint16_t force, float elapsedTime);
    void move(float angle, float pixelsPerSecond);
    void stopMoving();
    void clampToTopOf(AnimatedGraphic* otherGraphic);
    void resetGravityVelocity();
    bool isSubjectedToGravity();
    void jump(float force);
    void interruptJump();
    bool isFalling();
    void addSprite(std::string animationId, std::string filePath, uint16_t repeat);
    void addSprite(std::string animationId, std::string filePath);
    virtual void doOnInit() {};
    virtual void doOnUpdate(float elapsedTime) {};
    virtual void doOnFrame(float elapsedTime) {};
    virtual void hasCollidedRectLeft(AnimatedGraphic* otherGraphic) {};
    virtual void hasCollidedRectRight(AnimatedGraphic* otherGraphic) {};
    virtual void hasCollidedRectTop(AnimatedGraphic* otherGraphic) {};
    virtual void hasCollidedRectBottom(AnimatedGraphic* otherGraphic) {};
    virtual void hasCollidedRect(AnimatedGraphic* otherGraphic, Geo2D::Side side) {};
    
public:
    AnimatedGraphic(Vector2D position, float scale, int rotationAngle, float imageSwapIntervalInSeconds, bool applyGravity);
    ~AnimatedGraphic();
    Vector2D& getPosition();
    Vector2D& getMoveVelocity();
    Vector2D getVelocity();
    float getScale();
    int getWidth();
    int getSingleWidth();
    int getHeight();
    float getRotationAngle();
    void setRotationAngle(float angle);
    void setDeleteOnLeaveScreen(bool deleteOnLeaveScreen);
    bool isMarkedForDeletion();
    bool checkCollisionRect(AnimatedGraphic* otherGraphicp, float elapsedTime);
    Drawable* getActiveDrawable();
    std::list<Drawable*> getAllDrawables();
    void markForDeletion();
    void setX(float x);
    void setY(float y);
    void hide();
    void show();
    bool isPositionedWithinScreen();
    bool isVisible();
    void setPosition(Vector2D position);
    void addToPosition(Vector2D delta);
    void onInit();
    void onFrame(float elapsedTime);
    void onUpdate(float elapsedTime);
    virtual Vector2D getRotationPoint();
    virtual uint16_t getZIndex() {
        return 0;
    }
    void addShape(std::string animationId, Shape* shape);
    void onKeyboardLeft(jimp::KeyState keyState) {};
    void onKeyboardRight(jimp::KeyState keyState) {};
    void onKeyboardUp(jimp::KeyState keyState) {};
    void onKeyboardSpaceBar(jimp::KeyState keyState) {};
};
}

#endif
