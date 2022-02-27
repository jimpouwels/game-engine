#include <vector>
#include <map>
#include <list>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "graphic.hpp"
#include "animation.hpp"
#include "mathUtils.hpp"
#include "gameEngine.hpp"
#include "shape.hpp"
#include "sprite.hpp"

namespace jimp {

Graphic::Graphic(Vector2D position, float scale, int rotationAngle, float spriteSwapIntervalInSeconds) {
    this->position = position;
    this->scale = scale;
    animationMap = new std::map<std::string, Animation*>;
    this->drawableSwapIntervalInSeconds = spriteSwapIntervalInSeconds;
    GameEngine::getInstance()->registerGraphic(this);
    this->angle = rotationAngle;
}

Graphic::~Graphic() {
    for (const auto& [animationId, animation]: *animationMap) {
        delete animation;
    }
    delete animationMap;
    delete lock;
}

void Graphic::onInit() {
    doOnInit();
}

void Graphic::onFrame(float elapsedTime) {
    lock->lock();
    doOnFrame(elapsedTime);
    lock->unlock();
}

void Graphic::onUpdate(float elapsedTime) {
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

void Graphic::markForDeletion() {
    markedForDeletion = true;
}

void Graphic::markAsInitialized() {
    initialized = true;
}

bool Graphic::isInitialized() {
    return initialized;
}

void Graphic::setDeleteOnLeaveScreen(bool deleteOnLeaveScreen) {
    this->deleteOnLeaveScreen = deleteOnLeaveScreen;
}

Vector2D Graphic::getRotationPoint() {
    return Vector2D { getWidth() / 2.0F, .y = getHeight() / 2.0F };
}

Drawable* Graphic::getActiveDrawable() {
    return activeAnimation->getActiveDrawable();
}

void Graphic::setCurrentAnimation(std::string animationId) {
    if (activeAnimation->getId().compare(animationId) != 0) {
        activeAnimation = animationMap->find(animationId)->second;
        updateCurrentDrawableData();
    }
}

bool Graphic::checkCollisionRect(Graphic* otherGraphic) {
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

void Graphic::updateAnimation(float elapsedTime) {
    elapsedTimeSinceLastSwap += elapsedTime;
    if (drawableSwapIntervalInSeconds >= 0 && elapsedTimeSinceLastSwap >= drawableSwapIntervalInSeconds) {
        activeAnimation->switchToNextDrawable();
        elapsedTimeSinceLastSwap = 0;
    }
    updateCurrentDrawableData();
}

void Graphic::setX(float x) {
    position.x = x;
}

void Graphic::setY(float y) {
    position.y = y;
}

Vector2D& Graphic::getPosition() {
    return position;
}

Vector2D& Graphic::getVelocity() {
    return velocity;
}

void Graphic::setPosition(Vector2D position) {
    this->position = position;
}

void Graphic::addToPosition(Vector2D delta) {
    position.x += delta.x;
    position.y += delta.y;
}

int Graphic::getWidth() {
    return getActiveDrawable()->getWidth();
}

int Graphic::getHeight() {
    return getActiveDrawable()->getHeight();
}

float Graphic::getScale() {
    return scale;
}

float Graphic::getRotationAngle() {
    return angle;
}

std::list<Drawable*> Graphic::getAllDrawables() {
    std::list<Drawable*> allDrawables = std::list<Drawable*>();
    for (const auto& [animationId, animation]: *animationMap) {
        for (const auto& drawable: animation->getAllDrawables()) {
            allDrawables.push_back(drawable);
        }
    }
    return allDrawables;
}

void Graphic::accelerate(float angle, uint16_t mass, uint16_t force) {
    this->isAccelerating = true;
    this->velocityAngle = jimp::Geo2D::normalizeAngle(angle);
    this->mass = mass;
    this->moveForce = force;
}

void Graphic::setRotationAngle(float angle) {
    this->angle = angle;
}

bool Graphic::isMarkedForDeletion() {
    return markedForDeletion;
}

uint16_t Graphic::getVelocityAngle() {
    return velocityAngle;
}

void Graphic::updateMovement(float elapsedTime) {
    if (isAccelerating) {
        velocity = velocity + jimp::Geo2D::vectorFrom(moveForce, velocityAngle, mass, elapsedTime);
        isAccelerating = false;
    }
    addToPosition(jimp::Timing::toValueForElapsedTime(velocity, elapsedTime));
}

void Graphic::updateCurrentDrawableData() {
    Drawable* activeDrawable = getActiveDrawable();
    activeDrawable->setPosition(position);
    activeDrawable->setScale(scale);
    activeDrawable->setRotationAngle(angle);
    activeDrawable->setRotationPoint(getRotationPoint());
}

void Graphic::addSprite(std::string animationId, std::string filePath) {
    Sprite* sprite = new Sprite(position.x, position.y, scale, filePath);
    addDrawable(animationId, sprite);
}

void Graphic::addShape(std::string animationId, Shape* shape) {
    shape->setScale(scale);
    shape->setRotationAngle(angle);
    shape->setPosition(position);
    addDrawable(animationId, shape);
}

void Graphic::addDrawable(std::string animationId, Drawable* drawable) {
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
