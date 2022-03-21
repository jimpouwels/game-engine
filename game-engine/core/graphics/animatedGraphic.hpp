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
#include "color.hpp"

namespace jimp {

class AnimatedGraphic : public KeyListener {
    
private:
    Vector2D position = Vector2D::empty();
    float scale = 1.0f;
    float angle;
    uint16_t zIndex;
    bool visible = true;
    bool applyScrolling = false;
    bool applyGravity = false;
    bool interruptGravity = false;
    bool interruptMovementX = false;
    bool collidable = false;
    float transparency = 100.0f;
    bool inversedHorizontally = false;
    int collisionBoxWidth = 0;
    int collisionBoxHeight = 0;
    int marginLeft = 0;
    int marginRight = 0;
    int marginTop = 0;
    int marginBottom = 0;
    float totalRgbAnimationTime = 0;
    int requestedRgbAnimationTime = -1;
    Color rgb = Color::full();
    Color originalRgb = Color::full();
    Color targetRgb = Color::black();
    std::map<std::string, Animation*>* animationMap;
    Animation* activeAnimation = nullptr;
    bool markedForDeletion = false;
    bool deleteOnLeaveScreen = false;
    Vector2D moveVelocity = Vector2D::empty();
    Vector2D gravityVelocity = Vector2D::empty();
    std::mutex* processingLock = new std::mutex();
    std::mutex* deleteLock = new std::mutex();
    float elapsedTimeSinceLastSwap;
    void animateRgb(float elapsedTime);
    Vector2D calculateNextPosition(float elapsedTime);
    void resetGravityVelocity();
    void updateCurrentDrawableData();
    void updateAnimation(float elapsedTime);
    void updateMovement(float elapsedTime);
    void draw(float elapsedTime);
    void addDrawable(std::string animationId, Drawable* drawable);
    
protected:
    std::string name = "";
    void setCurrentAnimation(std::string animationId);
    std::string getCurrentAnimationId();
    void accelerate(float angle, uint16_t mass, uint16_t force, float elapsedTime);
    void move(float angle, float pixelsPerSecond);
    void stopMoving();
    void stayOnTopOf(AnimatedGraphic* otherGraphic);
    void stayToLeftOf(AnimatedGraphic* otherGraphic);
    void stayToRightOf(AnimatedGraphic* otherGraphic);
    void jump(float force);
    void interruptJump();
    virtual void doOnUpdate(float elapsedTime) {};
    virtual void doOnFrame(float elapsedTime) {};
    virtual void hasCollidedRectLeft(AnimatedGraphic* otherGraphic) {};
    virtual void hasCollidedRectRight(AnimatedGraphic* otherGraphic) {};
    virtual void hasCollidedRectTop(AnimatedGraphic* otherGraphic) {};
    virtual void hasCollidedRectBottom(AnimatedGraphic* otherGraphic) {};
    virtual void hasCollidedRect(AnimatedGraphic* otherGraphic, Geo2D::Side side) {};
    
public:
    AnimatedGraphic();
    ~AnimatedGraphic();
    Vector2D& getPosition();
    Vector2D getScreenPosition();
    Vector2D& getMoveVelocity();
    Vector2D getVelocity();
    std::string getName();
    void setName(std::string name);
    float getScale();
    void setScale(float scale);
    int getWidth();
    int getSingleWidth();
    int getHeight();
    int getCollisionRectWidth();
    int getCollisionRectHeight();
    int getMarginLeft();
    int getMarginRight();
    int getMarginTop();
    int getMarginBottom();
    float getScreenPositionBottom();
    float getScreenPositionTop();
    float getScreenPositionLeft();
    float getScreenPositionRight();
    float getWorldPositionBottom();
    float getWorldPositionTop();
    float getWorldPositionLeft();
    float getWorldPositionRight();
    float getRotationAngle();
    bool isCollidable();
    void lockForDeletion();
    void unlockForDeletion();
    float getTransparency();
    void setTransparency(float transparency);
    bool canCollideWith(AnimatedGraphic* otherGraphic, float elapsedTime);
    void drawInversedHorizontally(bool inverse);
    void setCollidable(bool collidable);
    void setSpriteSwapInterval(std::string animationId, float swapInterval);
    void setRotationAngle(float angle);
    void setDeleteOnLeaveScreen(bool deleteOnLeaveScreen);
    bool drawInversedHorizontally();
    bool isMarkedForDeletion();
    void animateRgbLevels(Color to, int seconds);
    void setRgbLevels(Color rgb);
    Color getRgbLevels();
    void setApplyScrolling(bool applyScrolling);
    void setApplyGravity(bool gravity);
    void checkCollisionRect(AnimatedGraphic* otherGraphicp, float elapsedTime);
    Drawable* getActiveDrawable();
    std::list<Drawable*> getAllDrawables();
    void markForDeletion();
    void setX(float x);
    void setY(float y);
    bool isPositionedWithinScreen();
    void setPosition(Vector2D position);
    void addToPosition(Vector2D delta);
    void onInit();
    void onFrame(float elapsedTime);
    void onUpdate(float elapsedTime);
    virtual Vector2D getRotationPoint();
    uint16_t getZIndex();
    void setZIndex(uint16_t zIndex);
    void addShape(std::string animationId, Shape* shape);
    void addSprite(std::string animationId, std::string filePath, uint16_t repeat);
    void addSprite(std::string animationId, std::string filePath);
    void onKeyboardLeft(jimp::KeyState keyState) {};
    void onKeyboardRight(jimp::KeyState keyState) {};
    void onKeyboardUp(jimp::KeyState keyState) {};
    void onKeyboardSpaceBar(jimp::KeyState keyState) {};
};
}

#endif
