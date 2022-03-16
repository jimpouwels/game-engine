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

AnimatedGraphic::AnimatedGraphic() {
    animationMap = new std::map<std::string, Animation*>;
}

AnimatedGraphic::~AnimatedGraphic() {
    for (const auto& [animationId, animation]: *animationMap) {
        delete animation;
    }
    delete animationMap;
    delete lock;
}

void AnimatedGraphic::onFrame(float elapsedTime) {
    lock->lock();
    doOnFrame(elapsedTime);
    lock->unlock();
}

void AnimatedGraphic::onUpdate(float elapsedTime) {
    lock->lock();
    if (deleteOnLeaveScreen && !isPositionedWithinScreen()) {
        markForDeletion();
    } else {
        doOnUpdate(elapsedTime);
        updateMovement(elapsedTime);
        this->updateAnimation(elapsedTime);
    }
    animateRgb(elapsedTime);
    lock->unlock();
}

void AnimatedGraphic::animateRgb(float elapsedTime) {
    if (requestedRgbAnimationTime == -1) {
        return;
    }
    totalRgbAnimationTime += elapsedTime;
    float remainingSeconds = requestedRgbAnimationTime - totalRgbAnimationTime;
    if ((targetRgb.r > originalRgb.r && rgb.r >= targetRgb.r)
        || (originalRgb.r > targetRgb.r && rgb.r <= targetRgb.r)
        || remainingSeconds <= 0.0F) {
        std::swap(targetRgb, originalRgb);
        totalRgbAnimationTime = 0;
        remainingSeconds = requestedRgbAnimationTime;
    }
    
    float rDelta = jimp::Timing::toValueForElapsedTime((targetRgb.r - rgb.r) / remainingSeconds, elapsedTime);
    float gDelta = jimp::Timing::toValueForElapsedTime((targetRgb.g - rgb.g) / remainingSeconds, elapsedTime);
    float bDelta = jimp::Timing::toValueForElapsedTime((targetRgb.b - rgb.b) / remainingSeconds, elapsedTime);
    
    rgb.r = fmax(0, (fmin(255, (rgb.r + rDelta))));
    rgb.g = fmax(0, (fmin(255, (rgb.g + gDelta))));
    rgb.b = fmax(0, (fmin(255, (rgb.b + bDelta))));
}

void AnimatedGraphic::markForDeletion() {
    markedForDeletion = true;
}

void AnimatedGraphic::setDeleteOnLeaveScreen(bool deleteOnLeaveScreen) {
    this->deleteOnLeaveScreen = deleteOnLeaveScreen;
}

Vector2D AnimatedGraphic::getRotationPoint() {
    return Vector2D::from(getWidth() / 2.0F, getHeight() / 2.0F);
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

void AnimatedGraphic::setZIndex(uint16_t zIndex) {
    this->zIndex = zIndex;
}

uint16_t AnimatedGraphic::getZIndex() {
    return zIndex;
}

bool AnimatedGraphic::canCollideWith(AnimatedGraphic *otherGraphic, float elapsedTime) {
    Vector2D currentGraphicNextPosition = calculateNextPosition(elapsedTime);
    float currentGraphicNextTop = currentGraphicNextPosition.y + getMarginTop();
    float currentGraphicNextBottom = currentGraphicNextPosition.y + getHeight() - getMarginBottom();
    float currentGraphicNextLeft = currentGraphicNextPosition.x + getMarginLeft();
    float currentGraphicNextRight = currentGraphicNextPosition.x + getWidth() - getMarginRight();
    
    Vector2D otherGraphicNextPosition = otherGraphic->calculateNextPosition(elapsedTime);
    float otherGraphicNextTop = otherGraphicNextPosition.y + otherGraphic->getMarginTop();
    float otherGraphicNextBottom = otherGraphicNextPosition.y + otherGraphic->getHeight() - otherGraphic->getMarginBottom();
    float otherGraphicNextLeft = otherGraphicNextPosition.x + otherGraphic->getMarginLeft();
    float otherGraphicNextRight = otherGraphicNextPosition.x + otherGraphic->getWidth() - otherGraphic->getMarginRight();
    
    if ((getScreenPositionRight() < otherGraphic->getScreenPositionLeft() && currentGraphicNextRight < otherGraphicNextLeft) ||
        (getScreenPositionLeft() > otherGraphic->getScreenPositionRight() && currentGraphicNextLeft > otherGraphicNextRight) ||
        (getScreenPositionBottom() < otherGraphic->getScreenPositionTop() && currentGraphicNextBottom < otherGraphicNextTop) ||
        (getScreenPositionTop() > otherGraphic->getScreenPositionBottom() && currentGraphicNextTop > otherGraphicNextBottom)) {
        return false;
    }
    return true;
}

void AnimatedGraphic::checkCollisionRect(AnimatedGraphic* otherGraphic, float elapsedTime) {
    Vector2D currentGraphicNextPosition = calculateNextPosition(elapsedTime);
    float currentGraphicNextTop = currentGraphicNextPosition.y + getMarginTop();
    float currentGraphicNextBottom = currentGraphicNextPosition.y + getHeight() - getMarginBottom();
    float currentGraphicNextLeft = currentGraphicNextPosition.x + getMarginLeft();
    float currentGraphicNextRight = currentGraphicNextPosition.x + getWidth() - getMarginRight();
    
    Vector2D otherGraphicNextPosition = otherGraphic->calculateNextPosition(elapsedTime);
    float otherGraphicNextTop = otherGraphicNextPosition.y + otherGraphic->getMarginTop();
    float otherGraphicNextBottom = otherGraphicNextPosition.y + otherGraphic->getHeight() - otherGraphic->getMarginBottom();
    float otherGraphicNextLeft = otherGraphicNextPosition.x + otherGraphic->getMarginLeft();
    float otherGraphicNextRight = otherGraphicNextPosition.x + otherGraphic->getWidth() - otherGraphic->getMarginRight();
    
    if ((MathUtils::smallerOrEquals(getScreenPositionBottom(), otherGraphic->getScreenPositionTop()) && currentGraphicNextBottom > otherGraphicNextTop)
            && !MathUtils::floatEquals(currentGraphicNextRight, otherGraphicNextLeft) && !MathUtils::floatEquals(currentGraphicNextLeft, otherGraphicNextRight)) {
        hasCollidedRect(otherGraphic, Geo2D::Side::BOTTOM);
        hasCollidedRectBottom(otherGraphic);
    } else if ((MathUtils::largerOrEquals(getScreenPositionTop(), otherGraphic->getScreenPositionBottom()) && currentGraphicNextTop < otherGraphicNextBottom)
            && !MathUtils::floatEquals(currentGraphicNextRight, otherGraphicNextLeft) && !MathUtils::floatEquals(currentGraphicNextLeft, otherGraphicNextRight)) {
        hasCollidedRect(otherGraphic, Geo2D::Side::TOP);
        hasCollidedRectTop(otherGraphic);
    } else if ((MathUtils::largerOrEquals(getScreenPositionLeft(), otherGraphic->getScreenPositionRight()) && currentGraphicNextLeft < otherGraphicNextRight)
               && !MathUtils::floatEquals(currentGraphicNextBottom, otherGraphicNextTop) && !MathUtils::floatEquals(currentGraphicNextTop, otherGraphicNextBottom)) {
        hasCollidedRect(otherGraphic, Geo2D::Side::LEFT);
        hasCollidedRectLeft(otherGraphic);
    } else if ((MathUtils::smallerOrEquals(getScreenPositionRight(), otherGraphic->getScreenPositionLeft()) && currentGraphicNextRight > otherGraphicNextLeft)
            && !MathUtils::floatEquals(currentGraphicNextBottom, otherGraphicNextTop) && !MathUtils::floatEquals(currentGraphicNextTop, otherGraphicNextBottom)) {
        hasCollidedRect(otherGraphic, Geo2D::Side::RIGHT);
        hasCollidedRectRight(otherGraphic);
    }
}

bool AnimatedGraphic::isPositionedWithinScreen() {
    return getActiveDrawable()->isPositionedWithinScreen();
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

void AnimatedGraphic::setName(std::string name) {
    this->name = name;
}

void AnimatedGraphic::drawInversedHorizontally(bool value) {
    inversedHorizontally = value;
}

bool AnimatedGraphic::drawInversedHorizontally() {
    return inversedHorizontally;
}

void AnimatedGraphic::setSpriteSwapInterval(float swapInterval) {
    this->drawableSwapIntervalInSeconds = swapInterval;
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

Vector2D AnimatedGraphic::getScreenPosition() {
    return position + (isApplyScrolling() ? ScrollingWorld::getInstance()->getOffset() : Vector2D::empty());
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

float AnimatedGraphic::getScreenPositionTop() {
    return getScreenPosition().y + getMarginTop();
}

float AnimatedGraphic::getScreenPositionBottom() {
    return getScreenPosition().y + getHeight() - getMarginBottom();
}

float AnimatedGraphic::getScreenPositionRight() {
    return getScreenPosition().x + getWidth() - getMarginRight();
}

float AnimatedGraphic::getScreenPositionLeft() {
    return getPosition().x + getMarginLeft();
}

float AnimatedGraphic::getWorldPositionTop() {
    return getPosition().y + getMarginTop();
}

float AnimatedGraphic::getWorldPositionBottom() {
    return getPosition().y + getHeight() - getMarginBottom();
}

float AnimatedGraphic::getWorldPositionRight() {
    return getPosition().x + getWidth() - getMarginRight();
}

float AnimatedGraphic::getWorldPositionLeft() {
    return getPosition().x + getMarginLeft();
}

float AnimatedGraphic::getScale() {
    return scale;
}

void AnimatedGraphic::setScale(float scale) {
    this->scale = scale;
}

float AnimatedGraphic::getRotationAngle() {
    return angle;
}

bool AnimatedGraphic::isApplyScrolling() {
    return applyScrolling || (ScrollingWorld::getInstance()->getMainCharacter() == this && GameEngine::getInstance()->isEditMode());
}

void AnimatedGraphic::setApplyGravity(bool gravity) {
    this->applyGravity = gravity;
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
    moveVelocity.x = 0;
    moveVelocity.y = 0;
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
    gravityVelocity.y = 0.0F;
}

void AnimatedGraphic::setRotationAngle(float angle) {
    this->angle = angle;
}

bool AnimatedGraphic::isMarkedForDeletion() {
    return markedForDeletion;
}

void AnimatedGraphic::setRgbLevels(Color rgb) {
    this->rgb = rgb;
}

Color AnimatedGraphic::getRgbLevels() {
    return rgb;
}

void AnimatedGraphic::animateRgbLevels(Color to, int seconds) {
    this->requestedRgbAnimationTime = seconds;
    targetRgb = to;
    originalRgb = rgb;
    totalRgbAnimationTime = 0;
}

void AnimatedGraphic::stayOnTopOf(AnimatedGraphic *otherGraphic) {
    float otherGraphicPositionTop = isApplyScrolling() ? otherGraphic->getWorldPositionTop() : otherGraphic->getScreenPositionTop();
    getPosition().y = otherGraphicPositionTop - (getHeight() - getMarginBottom());
    resetGravityVelocity();
    interruptGravity = true;
}

void AnimatedGraphic::stayToLeftOf(AnimatedGraphic *otherGraphic) {
    float otherGraphicPositionLeft = isApplyScrolling() ? otherGraphic->getWorldPositionLeft() : otherGraphic->getScreenPositionLeft();
    getPosition().x = otherGraphicPositionLeft - (getWidth() - getMarginRight());
    interruptMovementX = true;
}

void AnimatedGraphic::stayToRightOf(AnimatedGraphic *otherGraphic) {
    float otherGraphicPositionRight = isApplyScrolling() ? otherGraphic->getWorldPositionRight() : otherGraphic->getScreenPositionRight();
    getPosition().x = otherGraphicPositionRight - getMarginLeft();
    interruptMovementX = true;
}

void AnimatedGraphic::updateMovement(float elapsedTime) {
    if (applyGravity && !interruptGravity)  {
        gravityVelocity.y += jimp::Timing::toValueForElapsedTime(GameEngine::getInstance()->getGravityForce(), elapsedTime);
    }
    interruptGravity = false;
    
    addToPosition(jimp::Timing::toValueForElapsedTime(gravityVelocity, elapsedTime));
    
    if (interruptMovementX) {
        interruptMovementX = false;
        addToPosition(jimp::Timing::toValueForElapsedTime(Vector2D::from(0, moveVelocity.y), elapsedTime));
    } else {
        addToPosition(jimp::Timing::toValueForElapsedTime(moveVelocity, elapsedTime));
    }
}

void AnimatedGraphic::updateCurrentDrawableData() {
    Drawable* activeDrawable = getActiveDrawable();
    activeDrawable->setPosition(getScreenPosition());
    activeDrawable->setScale(scale);
    activeDrawable->setRotationAngle(angle);
    activeDrawable->setRotationPoint(getRotationPoint());
    Sprite* sprite = dynamic_cast<Sprite*>(activeDrawable);
    if (sprite != nullptr) {
        sprite->setDrawInversedHorizontally(inversedHorizontally);
        sprite->setRgbLevels(rgb);
    }
}

Vector2D AnimatedGraphic::calculateNextPosition(float elapsedTime) {
    Vector2D velocity = Vector2D::empty();
    if (applyGravity && !interruptGravity)  {
        velocity.y = gravityVelocity.y;
        velocity.y += jimp::Timing::toValueForElapsedTime(GameEngine::getInstance()->getGravityForce(), elapsedTime);
    }
    if (!interruptMovementX) {
        velocity.x += moveVelocity.x;
    }
    velocity.y += moveVelocity.y;
    return getScreenPosition() + jimp::Timing::toValueForElapsedTime(velocity, elapsedTime);
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
        Sprite* sprite = dynamic_cast<Sprite*>(drawable);
        if (sprite != nullptr) {
            marginLeft = sprite->getMarginLeft();
            marginRight = sprite->getMarginRight();
            marginTop = sprite->getMarginTop();
            marginBottom = sprite->getMarginBottom();
        }
        collisionBoxWidth = getActiveDrawable()->getWidth() - getMarginLeft() - getMarginRight();
        collisionBoxHeight = getActiveDrawable()->getHeight() - getMarginTop() - getMarginBottom();
    }
}

}
