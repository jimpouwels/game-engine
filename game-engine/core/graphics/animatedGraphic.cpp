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

bool AnimatedGraphic::checkCollisionRect(AnimatedGraphic* otherGraphic) {
    Vector2D otherGraphicPosition = otherGraphic->getPosition();
    
    if (abs((getPosition().x + getWidth()) - otherGraphicPosition.x) <= 2 &&
        (((jimp::MathUtils::isBetween(getPosition().y, otherGraphicPosition.y, otherGraphicPosition.y + otherGraphic->getHeight())) ||
         ((jimp::MathUtils::isBetween(getPosition().y + getHeight(), otherGraphic->getPosition().y, otherGraphic->getPosition().y + otherGraphic->getHeight())))) ||
        ((jimp::MathUtils::isBetween(otherGraphicPosition.y, getPosition().y, getPosition().y + getHeight())) ||
         ((jimp::MathUtils::isBetween(otherGraphicPosition.y + otherGraphic->getHeight(), getPosition().y, getPosition().y + getHeight())))))) {
        hasCollidedRect(otherGraphic, Geo2D::Side::RIGHT);
        hasCollidedRectRight(otherGraphic);
        otherGraphic->hasCollidedRect(this, Geo2D::Side::LEFT);
        otherGraphic->hasCollidedRectLeft(this);
        return true;
    } else if (abs(getPosition().x - (otherGraphicPosition.x + otherGraphic->getWidth())) <= 2 &&
            (((jimp::MathUtils::isBetween(otherGraphicPosition.y, getPosition().y, getPosition().y + getHeight())) ||
             (jimp::MathUtils::isBetween(otherGraphicPosition.y + otherGraphic->getHeight(), getPosition().y, getPosition().y + getHeight()))) ||
               ((jimp::MathUtils::isBetween(getPosition().y, otherGraphicPosition.y, otherGraphicPosition.y + otherGraphic->getHeight())) ||
                (jimp::MathUtils::isBetween(getPosition().y + getHeight(), otherGraphicPosition.y, otherGraphicPosition.y + otherGraphic->getHeight()))))) {
        hasCollidedRect(otherGraphic, Geo2D::Side::LEFT);
        hasCollidedRectLeft(otherGraphic);
        otherGraphic->hasCollidedRect(this, Geo2D::Side::RIGHT);
        otherGraphic->hasCollidedRectRight(this);
        return true;
    } else if (abs((getPosition().y + getHeight()) - otherGraphicPosition.y) <= 2 &&
            (((jimp::MathUtils::isBetween(getPosition().x, otherGraphicPosition.x, otherGraphicPosition.x + otherGraphic->getWidth()) ||
             (jimp::MathUtils::isBetween(getPosition().x + getWidth(), otherGraphicPosition.x, otherGraphicPosition.x + otherGraphic->getWidth())))) ||
               ((jimp::MathUtils::isBetween(otherGraphicPosition.x, getPosition().x, getPosition().x + getWidth()) ||
                (jimp::MathUtils::isBetween(otherGraphicPosition.x + otherGraphic->getWidth(), getPosition().x, getPosition().x + getWidth())))))) {
        hasCollidedRect(otherGraphic, Geo2D::Side::BOTTOM);
        hasCollidedRectBottom(otherGraphic);
        otherGraphic->hasCollidedRect(this, Geo2D::Side::TOP);
        otherGraphic->hasCollidedRectTop(this);
        return true;
    } else if (abs(getPosition().y - (otherGraphicPosition.y + otherGraphic->getHeight())) <= 2 &&
            (((jimp::MathUtils::isBetween(otherGraphicPosition.x, getPosition().x, getPosition().x + getWidth()) ||
             (jimp::MathUtils::isBetween(otherGraphicPosition.x + otherGraphic->getWidth(), getPosition().x, getPosition().x + getWidth())))) ||
               ((jimp::MathUtils::isBetween(getPosition().x, otherGraphicPosition.x, otherGraphicPosition.x + otherGraphic->getWidth()) ||
                (jimp::MathUtils::isBetween(getPosition().x + getWidth(), otherGraphicPosition.x, otherGraphicPosition.x + otherGraphic->getWidth())))))) {
        hasCollidedRect(otherGraphic, Geo2D::Side::TOP);
        hasCollidedRectTop(otherGraphic);
        otherGraphic->hasCollidedRect(this, Geo2D::Side::BOTTOM);
        otherGraphic->hasCollidedRectBottom(this);
        return true;
    }
    
    return false;
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

Vector2D& AnimatedGraphic::getJumpVelocity() {
    return jumpVelocity;
}

void AnimatedGraphic::setPosition(Vector2D position) {
    this->position = position;
}

void AnimatedGraphic::addToPosition(Vector2D delta) {
    position.x += delta.x;
    position.y += delta.y;
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
    moving = true;
    moveForce = pixelsPerSecond;
    moveAngle = angle;
}

void AnimatedGraphic::stopMoving() {
    moveVelocity.x = 0;
    moveVelocity.y = 0;
    moving = false;
}

void AnimatedGraphic::jump(float force) {
    jumpVelocity.y = -force;
}

void AnimatedGraphic::interruptJump() {
    if (jumpVelocity.y < 0) {
        jumpVelocity.y = -jumpVelocity.y;
    }
}

void AnimatedGraphic::blockGravity() {
    if (jumpVelocity.y > 0) {
        gravityBlocked = true;
        jumpVelocity.y = 0;
    }
}

void AnimatedGraphic::unblockGravity() {
    gravityBlocked = false;
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

bool AnimatedGraphic::isVisible() {
    return visible;
}

void AnimatedGraphic::updateMovement(float elapsedTime) {
    // ========= JUMPING
    if (applyGravity)  {
        jumpVelocity.y += GameEngine::getInstance()->getGravityForce();
    }
    if (gravityBlocked && isFalling()) {
        jumpVelocity.y = 0;
    }
    
    addToPosition(jimp::Timing::toValueForElapsedTime(jumpVelocity, elapsedTime));
    
    // ========= MOVEMENT
    if (moving) {
        moveVelocity = jimp::Geo2D::vectorFrom(moveAngle, moveForce);
    }
    addToPosition(jimp::Timing::toValueForElapsedTime(moveVelocity, elapsedTime));
}

bool AnimatedGraphic::isJumping() {
    return jumpVelocity.y != 0;
}

bool AnimatedGraphic::isFalling() {
    return jumpVelocity.y > 0;
}

void AnimatedGraphic::updateCurrentDrawableData() {
    Drawable* activeDrawable = getActiveDrawable();
    activeDrawable->setPosition(position);
    activeDrawable->setScale(scale);
    activeDrawable->setRotationAngle(angle);
    activeDrawable->setRotationPoint(getRotationPoint());
}

void AnimatedGraphic::addSprite(std::string animationId, std::string filePath) {
    Sprite* sprite = new Sprite(position.x, position.y, scale, filePath);
    addDrawable(animationId, sprite);
}

void AnimatedGraphic::addShape(std::string animationId, Shape* shape) {
    shape->setScale(scale);
    shape->setRotationAngle(angle);
    shape->setPosition(position);
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
