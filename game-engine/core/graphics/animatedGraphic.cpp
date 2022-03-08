#include <vector>
#include <map>
#include <list>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "animatedGraphic.hpp"
#include "animation.hpp"
#include "mathUtils.hpp"
#include "gameEngine.hpp"
#include "shape.hpp"
#include "sprite.hpp"

namespace jimp {

AnimatedGraphic::AnimatedGraphic(Vector2D position, float scale, int rotationAngle, float spriteSwapIntervalInSeconds, bool applyGravity) {
    this->position = position;
    this->scale = scale;
    this->applyGravity = applyGravity;
    animationMap = new std::map<std::string, Animation*>;
    this->drawableSwapIntervalInSeconds = spriteSwapIntervalInSeconds;
    GameEngine::getInstance()->registerGraphic(this);
    this->angle = rotationAngle;
}

AnimatedGraphic::~AnimatedGraphic() {
    for (const auto& [animationId, animation]: *animationMap) {
        delete animation;
    }
    delete animationMap;
    delete lock;
}

void AnimatedGraphic::onInit() {
    doOnInit();
}

void AnimatedGraphic::onFrame(float elapsedTime) {
    lock->lock();
    doOnFrame(elapsedTime);
    lock->unlock();
}

void AnimatedGraphic::onUpdate(float elapsedTime) {
    lock->lock();
    if (deleteOnLeaveScreen && !GameEngine::getInstance()->isPositionWithinScreen(getPosition())) {
        markForDeletion();
    } else {
        doOnUpdate(elapsedTime);
        updateMovement(elapsedTime);
        this->updateAnimation(elapsedTime);
    }
    lock->unlock();
}

void AnimatedGraphic::markForDeletion() {
    markedForDeletion = true;
}

void AnimatedGraphic::setDeleteOnLeaveScreen(bool deleteOnLeaveScreen) {
    this->deleteOnLeaveScreen = deleteOnLeaveScreen;
}

Vector2D AnimatedGraphic::getRotationPoint() {
    return Vector2D { getWidth() / 2.0F, .y = getHeight() / 2.0F };
}

Drawable* AnimatedGraphic::getActiveDrawable() {
    return activeAnimation->getActiveDrawable();
}

void AnimatedGraphic::setCurrentAnimation(std::string animationId) {
    if (activeAnimation->getId().compare(animationId) != 0) {
        activeAnimation = animationMap->find(animationId)->second;
        updateCurrentDrawableData();
    }
}

std::string AnimatedGraphic::getCurrentAnimationId() {
    return activeAnimation->getId();
}

void AnimatedGraphic::checkCollisionRect(AnimatedGraphic* otherGraphic, float elapsedTime) {
    if (!isCollidable() || !otherGraphic->isCollidable()) {
        return;
    }
    
    Vector2D currentGraphicCurrentPosition = getPosition();
    Vector2D currentGraphicNextPosition = calculateNextPosition(elapsedTime);
    
    Vector2D otherGraphicCurrentPosition = otherGraphic->getPosition();
    Vector2D otherGraphicNextPosition = otherGraphic->calculateNextPosition(elapsedTime);
     
    if ((((currentGraphicCurrentPosition.y + getHeight() - getMarginBottom() <= otherGraphicCurrentPosition.y + otherGraphic->getMarginTop() && currentGraphicNextPosition.y + getHeight() - getMarginBottom() > otherGraphicNextPosition.y + otherGraphic->getMarginTop()))
         && (((currentGraphicNextPosition.x + getMarginLeft() > otherGraphicNextPosition.x + otherGraphic->marginLeft
                && currentGraphicNextPosition.x + getMarginLeft() < otherGraphicNextPosition.x + otherGraphic->getWidth() - otherGraphic->getMarginRight()))
           || (currentGraphicNextPosition.x + getWidth() - getMarginRight() > otherGraphicNextPosition.x + otherGraphic->getMarginLeft()
               && currentGraphicNextPosition.x + getWidth() - getMarginRight() < otherGraphicNextPosition.x + otherGraphic->getWidth() - otherGraphic->getMarginRight())
             || (currentGraphicNextPosition.x + getMarginLeft() < otherGraphicNextPosition.x + otherGraphic->getMarginLeft()
             && currentGraphicNextPosition.x + getWidth() - getMarginRight() > otherGraphicNextPosition.x + otherGraphic->getWidth() - otherGraphic->getMarginRight())))) {
        hasCollidedRect(otherGraphic, Geo2D::Side::BOTTOM);
        hasCollidedRectBottom(otherGraphic);
    } else if ((((currentGraphicCurrentPosition.y + getMarginTop() >= otherGraphicCurrentPosition.y + otherGraphic->getHeight() - otherGraphic->getMarginBottom() && currentGraphicNextPosition.y + getMarginTop() < otherGraphicNextPosition.y + otherGraphic->getHeight() - otherGraphic->getMarginBottom())) &&
         (((currentGraphicNextPosition.x + getMarginLeft() > otherGraphicNextPosition.x + otherGraphic->marginLeft
               && currentGraphicNextPosition.x + getMarginLeft() < otherGraphicNextPosition.x + otherGraphic->getWidth() - otherGraphic->getMarginRight()))
          || (currentGraphicNextPosition.x + getWidth() - getMarginRight() > otherGraphicNextPosition.x + otherGraphic->getMarginLeft()
              && currentGraphicNextPosition.x + getWidth() - getMarginRight() < otherGraphicNextPosition.x + otherGraphic->getWidth() - otherGraphic->getMarginRight())
          || (currentGraphicNextPosition.x + getMarginLeft() < otherGraphicNextPosition.x + otherGraphic->getMarginLeft()
          && currentGraphicNextPosition.x + getWidth() - getMarginRight() > otherGraphicNextPosition.x + otherGraphic->getWidth() - otherGraphic->getMarginRight())))) {
        hasCollidedRect(otherGraphic, Geo2D::Side::TOP);
        hasCollidedRectTop(otherGraphic);
    } else if ((((currentGraphicCurrentPosition.x + getWidth() - getMarginRight() <= otherGraphicCurrentPosition.x + otherGraphic->getMarginLeft() && currentGraphicNextPosition.x + getWidth() - getMarginRight() >= otherGraphicNextPosition.x + otherGraphic->getMarginLeft()))
         && (((currentGraphicNextPosition.y + getMarginTop() > otherGraphicNextPosition.y + otherGraphic->marginTop
              && currentGraphicNextPosition.y + getMarginTop() < otherGraphicNextPosition.y + otherGraphic->getHeight() - otherGraphic->getMarginBottom()))
         || (currentGraphicNextPosition.y + getHeight() - getMarginBottom() > otherGraphicNextPosition.y + otherGraphic->getMarginTop()
             && currentGraphicNextPosition.y + getHeight() - getMarginBottom() < otherGraphicNextPosition.y + otherGraphic->getHeight() - otherGraphic->getMarginBottom())))) {
        hasCollidedRect(otherGraphic, Geo2D::Side::RIGHT);
        hasCollidedRectRight(otherGraphic);
    } else if ((((currentGraphicCurrentPosition.x + getMarginLeft() >= otherGraphicCurrentPosition.x + otherGraphic->getWidth() - otherGraphic->getMarginRight() && currentGraphicNextPosition.x + getMarginLeft() <= otherGraphicNextPosition.x + otherGraphic->getWidth() - otherGraphic->getMarginRight()))
         && (((currentGraphicNextPosition.y + getMarginTop() > otherGraphicNextPosition.y + otherGraphic->marginTop
                && currentGraphicNextPosition.y + getMarginTop() < otherGraphicNextPosition.y + otherGraphic->getHeight() - otherGraphic->getMarginBottom()))
           || (currentGraphicNextPosition.y + getHeight() - getMarginBottom() > otherGraphicNextPosition.y + otherGraphic->getMarginTop()
               && currentGraphicNextPosition.y + getHeight() - getMarginBottom() < otherGraphicNextPosition.y + otherGraphic->getHeight() - otherGraphic->getMarginBottom()))) && currentGraphicNextPosition.y != otherGraphicNextPosition.y) {
        hasCollidedRect(otherGraphic, Geo2D::Side::LEFT);
        hasCollidedRectLeft(otherGraphic);
    }
}

bool AnimatedGraphic::isPositionedWithinScreen() {
    if (getPosition().x + getWidth() < 0) {
        return false;
    }
    if (getPosition().x > GameEngine::getInstance()->getScreenWidth()) {
        return false;
    }
    if (getPosition().y + getHeight() < 0) {
        return false;
    }
    if (getPosition().y > GameEngine::getInstance()->getScreenHeight()) {
        return false;
    }
    return true;
}

void AnimatedGraphic::updateAnimation(float elapsedTime) {
    elapsedTimeSinceLastSwap += elapsedTime;
    if (drawableSwapIntervalInSeconds >= 0 && elapsedTimeSinceLastSwap >= drawableSwapIntervalInSeconds) {
        activeAnimation->switchToNextDrawable();
        elapsedTimeSinceLastSwap = 0;
    }
    updateCurrentDrawableData();
}

std::string AnimatedGraphic::getName() {
    return name;
}

void AnimatedGraphic::drawInversedHorizontally(bool value) {
    inversedHorizontally = value;
}

bool AnimatedGraphic::drawInversedHorizontally() {
    return inversedHorizontally;
}

void AnimatedGraphic::setX(float x) {
    position.x = x;
}

void AnimatedGraphic::setY(float y) {
    position.y = y;
}

Vector2D& AnimatedGraphic::getPosition() {
    return position;
}

Vector2D& AnimatedGraphic::getMoveVelocity() {
    return moveVelocity;
}

Vector2D AnimatedGraphic::getVelocity() {
    return gravityVelocity + moveVelocity;
}

void AnimatedGraphic::setPosition(Vector2D position) {
    this->position = position;
}

void AnimatedGraphic::addToPosition(Vector2D delta) {
    position.x += delta.x;
    position.y += delta.y;
}

bool AnimatedGraphic::isCollidable() {
    return collidable;
}

void AnimatedGraphic::setCollidable(bool collidable) {
    this->collidable = collidable;
}

int AnimatedGraphic::getSingleWidth() {
    return getActiveDrawable()->getSingleWidth();
}

int AnimatedGraphic::getWidth() {
    return getActiveDrawable()->getWidth();
}

int AnimatedGraphic::getHeight() {
    return getActiveDrawable()->getHeight();
}

int AnimatedGraphic::getCollisionRectWidth() {
    return collisionBoxWidth;
}

int AnimatedGraphic::getCollisionRectHeight() {
    return collisionBoxHeight;
}

float AnimatedGraphic::getScale() {
    return scale;
}

float AnimatedGraphic::getRotationAngle() {
    return angle;
}

bool AnimatedGraphic::isApplyScrolling() {
    return applyScrolling;
}

void AnimatedGraphic::setApplyScrolling(bool applyScrolling) {
    this->applyScrolling = applyScrolling;
}

std::list<Drawable*> AnimatedGraphic::getAllDrawables() {
    std::list<Drawable*> allDrawables = std::list<Drawable*>();
    for (const auto& [animationId, animation]: *animationMap) {
        for (const auto& drawable: animation->getAllDrawables()) {
            allDrawables.push_back(drawable);
        }
    }
    return allDrawables;
}

void AnimatedGraphic::accelerate(float angle, uint16_t mass, uint16_t force, float elapsedTime) {
    float normalizedAngle = jimp::Geo2D::normalizeAngle(angle);
    moveVelocity = moveVelocity + jimp::Geo2D::vectorFrom(force, normalizedAngle, mass, elapsedTime);
}

void AnimatedGraphic::move(float angle, float pixelsPerSecond) {
    moveVelocity = jimp::Geo2D::vectorFrom(angle, pixelsPerSecond);
}

void AnimatedGraphic::stopMoving() {
    moveVelocity.reset();
}

void AnimatedGraphic::jump(float force) {
    gravityVelocity.y = -force;
}

void AnimatedGraphic::interruptJump() {
    if (gravityVelocity.y < 0) {
        gravityVelocity.y = -gravityVelocity.y;
    }
}

void AnimatedGraphic::resetGravityVelocity() {
    gravityVelocity.y = 0;
}

void AnimatedGraphic::setRotationAngle(float angle) {
    this->angle = angle;
}

bool AnimatedGraphic::isMarkedForDeletion() {
    return markedForDeletion;
}

void AnimatedGraphic::hide() {
    visible = false;
}

void AnimatedGraphic::show() {
    visible = true;
}

void AnimatedGraphic::stayOnTopOf(AnimatedGraphic *otherGraphic) {
    getPosition().y = otherGraphic->getPosition().y - getHeight() + getMarginBottom() + otherGraphic->getMarginTop();
    resetGravityVelocity();
    interruptGravity = true;
}

void AnimatedGraphic::stayToLeftOf(AnimatedGraphic *otherGraphic) {
    getPosition().x = otherGraphic->getPosition().x + otherGraphic->getMarginLeft() - (getHeight() - getMarginRight());
    interruptMovement = true;
}

void AnimatedGraphic::stayToRightOf(AnimatedGraphic *otherGraphic) {
    getPosition().x = otherGraphic->getPosition().x + otherGraphic->getWidth() - otherGraphic->getMarginRight() - getMarginLeft();
    interruptMovement = true;
}

bool AnimatedGraphic::isVisible() {
    return visible;
}

void AnimatedGraphic::updateMovement(float elapsedTime) {
    if (applyGravity && !interruptGravity)  {
        gravityVelocity.y += GameEngine::getInstance()->getGravityForce();
    }
    interruptGravity = false;
    addToPosition(jimp::Timing::toValueForElapsedTime(gravityVelocity, elapsedTime));
    
    if (!interruptMovement) {
        addToPosition(jimp::Timing::toValueForElapsedTime(moveVelocity, elapsedTime));
    }
    interruptMovement = false;
}

void AnimatedGraphic::updateCurrentDrawableData() {
    Drawable* activeDrawable = getActiveDrawable();
    activeDrawable->setPosition(position);
    activeDrawable->setScale(scale);
    activeDrawable->setRotationAngle(angle);
    activeDrawable->setRotationPoint(getRotationPoint());
    Sprite* sprite = dynamic_cast<Sprite*>(activeDrawable);
    if (sprite != nullptr) {
        sprite->setDrawInversedHorizontally(inversedHorizontally);
    }
}

Vector2D AnimatedGraphic::calculateNextPosition(float elapsedTime) {
    Vector2D velocity = getVelocity();
    if (applyGravity && !interruptGravity)  {
        velocity.y += GameEngine::getInstance()->getGravityForce();
    }
    return getPosition() + jimp::Timing::toValueForElapsedTime(velocity, elapsedTime);
}

void AnimatedGraphic::addSprite(std::string animationId, std::string filePath) {
    addSprite(animationId, filePath, 1);
}

void AnimatedGraphic::addSprite(std::string animationId, std::string filePath, uint16_t repeat) {
    Sprite* sprite = new Sprite(position.x, position.y, scale, angle, repeat, filePath);
    addDrawable(animationId, sprite);
}

void AnimatedGraphic::addShape(std::string animationId, Shape* shape) {
    addDrawable(animationId, shape);
}

int AnimatedGraphic::getMarginLeft() {
    return marginLeft * getScale();
}

int AnimatedGraphic::getMarginRight() {
    return marginRight * getScale();
}

int AnimatedGraphic::getMarginTop() {
    return marginTop * getScale();
}

int AnimatedGraphic::getMarginBottom() {
    return marginBottom * getScale();
}

void AnimatedGraphic::addDrawable(std::string animationId, Drawable* drawable) {
    Animation* animation = nullptr;
    if (animationMap->find(animationId) == animationMap->end()) {
        animation = new Animation(animationId);
        animationMap->insert({animationId, animation});
    } else {
        animation = animationMap->find(animationId)->second;
    }
    animationMap->find(animationId)->second->addDrawable(drawable);
    if (activeAnimation == nullptr) {
        activeAnimation = animation;
        updateAnimation(0);
        collisionBoxWidth = getActiveDrawable()->getWidth() - getMarginLeft() - getMarginRight();
        collisionBoxHeight = getActiveDrawable()->getHeight() - getMarginTop() - getMarginBottom();
        Sprite* sprite = dynamic_cast<Sprite*>(drawable);
        if (sprite != nullptr) {
            marginLeft = sprite->getMarginLeft();
            marginRight = sprite->getMarginRight();
            marginTop = sprite->getMarginTop();
            marginBottom = sprite->getMarginBottom();
        }
    }
}

}
