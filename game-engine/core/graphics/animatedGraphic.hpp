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
    std::map<std::string, Animation*>* animationMap;
    Animation* activeAnimation = nullptr;
    bool markedForDeletion = false;
    bool deleteOnLeaveScreen = false;
    bool initialized = false;
    Vector2D velocity = Vector2D { .x = 0, .y = 0 };
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
    void move(float angle, float pixelsPerSecond, float elapsedTime);
    void markAsInitialized();
    virtual void doOnInit() { };
    virtual void doOnUpdate(float elapsedTime) { };
    virtual void doOnFrame(float elapsedTime) { };
    virtual void hasCollidedRectLeft(AnimatedGraphic* otherGraphic) {};
    virtual void hasCollidedRectRight(AnimatedGraphic* otherGraphic) {};
    virtual void hasCollidedRectTop(AnimatedGraphic* otherGraphic) {};
    virtual void hasCollidedRectBottom(AnimatedGraphic* otherGraphic) {};
    virtual void hasCollidedRect(AnimatedGraphic* otherGraphic, Geo2D::Side side) {};
    
public:
    AnimatedGraphic(Vector2D position, float scale, int rotationAngle, float imageSwapIntervalInSeconds);
    ~AnimatedGraphic();
    Vector2D& getPosition();
    Vector2D& getVelocity();
    float getScale();
    int getWidth();
    int getHeight();
    float getRotationAngle();
    void setRotationAngle(float angle);
    void setDeleteOnLeaveScreen(bool deleteOnLeaveScreen);
    bool isMarkedForDeletion();
    bool isInitialized();
    bool checkCollisionRect(AnimatedGraphic* otherGraphicp);
    Drawable* getActiveDrawable();
    std::list<Drawable*> getAllDrawables();
    void markForDeletion();
    void setX(float x);
    void setY(float y);
    void setPosition(Vector2D position);
    void addToPosition(Vector2D delta);
    void onInit();
    void onFrame(float elapsedTime);
    void onUpdate(float elapsedTime);
    virtual Vector2D getRotationPoint();
    virtual uint16_t getZIndex() {
        return 0;
    }
    void addSprite(std::string animationId, std::string filePath);
    void addShape(std::string animationId, Shape* shape);
    void onKeyboardLeft(jimp::KeyState keyState) {};
    void onKeyboardRight(jimp::KeyState keyState) {};
    void onKeyboardUp(jimp::KeyState keyState) {};
    void onKeyboardSpaceBar(jimp::KeyState keyState) {};
};
}

#endif
