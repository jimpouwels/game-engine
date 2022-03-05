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
    Vector2D otherGraphicPosition = otherGraphic->getPosition();
    Vector2D currentGraphicPosition = getPosition();
    
    Vector2D currentGraphicPreviousPosition = calculatePreviousPosition(elapsedTime);
    Vector2D currentGraphicNextPosition = getPosition();
    
    Vector2D otherGraphicPreviousPosition = otherGraphic->calculatePreviousPosition(elapsedTime);
    Vector2D otherGraphicNextPosition = otherGraphic->getPosition();
   
    if ((((currentGraphicPreviousPosition.y + getHeight() <= otherGraphicPreviousPosition.y && currentGraphicNextPosition.y + getHeight() > otherGraphicNextPosition.y) || currentGraphicPosition.y == otherGraphicPosition.y)
         && (currentGraphicNextPosition.x + getWidth() > otherGraphicNextPosition.x && currentGraphicPosition.x < otherGraphicNextPosition.x + otherGraphic->getWidth()))) {
        hasCollidedRect(otherGraphic, Geo2D::Side::BOTTOM);
        hasCollidedRectBottom(otherGraphic);
        otherGraphic->hasCollidedRect(this, Geo2D::Side::TOP);
        otherGraphic->hasCollidedRectTop(this);
    } else if ((((currentGraphicPreviousPosition.y >= otherGraphicPreviousPosition.y + otherGraphic->getHeight() && currentGraphicNextPosition.y < otherGraphicNextPosition.y + otherGraphic->getHeight()) || currentGraphicPosition.y == otherGraphicPosition.y) &&
        (currentGraphicNextPosition.x + getWidth() > otherGraphicNextPosition.x && currentGraphicPosition.x < otherGraphicNextPosition.x + otherGraphic->getWidth()))) {
        hasCollidedRect(otherGraphic, Geo2D::Side::TOP);
        hasCollidedRectTop(otherGraphic);
        otherGraphic->hasCollidedRect(this, Geo2D::Side::BOTTOM);
        otherGraphic->hasCollidedRectBottom(this);
    } else if ((((currentGraphicPreviousPosition.x + getWidth() <= otherGraphicPreviousPosition.x && currentGraphicNextPosition.x + getWidth() > otherGraphicNextPosition.x) || currentGraphicPosition.x == otherGraphicPosition.x)
         && (currentGraphicNextPosition.y + getHeight() > otherGraphicNextPosition.y && currentGraphicPosition.y < otherGraphicNextPosition.y + otherGraphic->getHeight()))) {
        hasCollidedRect(otherGraphic, Geo2D::Side::LEFT);
        hasCollidedRectLeft(otherGraphic);
        otherGraphic->hasCollidedRect(this, Geo2D::Side::RIGHT);
        otherGraphic->hasCollidedRectRight(this);
    } else if ((((currentGraphicPreviousPosition.x >= otherGraphicPreviousPosition.x + otherGraphic->getWidth() && currentGraphicNextPosition.x < otherGraphicNextPosition.x + otherGraphic->getWidth()) || currentGraphicPosition.x == otherGraphicPosition.x) &&
        (currentGraphicNextPosition.y + getHeight() > otherGraphicNextPosition.y && currentGraphicPosition.y < otherGraphicNextPosition.y + otherGraphic->getHeight()))) {
        hasCollidedRect(otherGraphic, Geo2D::Side::RIGHT);
        hasCollidedRectRight(otherGraphic);
        otherGraphic->hasCollidedRect(this, Geo2D::Side::LEFT);
        otherGraphic->hasCollidedRectLeft(this);
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
    getPosition().y = otherGraphic->getPosition().y - getHeight();
    resetGravityVelocity();
}

void AnimatedGraphic::stayToLeftOf(AnimatedGraphic *otherGraphic) {
    getPosition().x = otherGraphic->getPosition().x - getWidth();
}

void AnimatedGraphic::stayToRightOf(AnimatedGraphic *otherGraphic) {
    getPosition().x = otherGraphic->getPosition().x + otherGraphic->getWidth();
}

bool AnimatedGraphic::isVisible() {
    return visible;
}

void AnimatedGraphic::updateMovement(float elapsedTime) {
    if (applyGravity)  {
        gravityVelocity.y += GameEngine::getInstance()->getGravityForce();
    }
    addToPosition(jimp::Timing::toValueForElapsedTime(getVelocity(), elapsedTime));
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

Vector2D AnimatedGraphic::calculatePreviousPosition(float elapsedTime) {
    return getPosition() - jimp::Timing::toValueForElapsedTime(getVelocity(), elapsedTime);
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
    }
}

}
