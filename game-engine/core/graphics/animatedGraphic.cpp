#include <vector>
#include <map>
#include <list>
#include <iostream>
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
    delete deleteLock;
}

void AnimatedGraphic::onFrame(float elapsedTime) {
    doOnFrame(elapsedTime);
}

void AnimatedGraphic::onUpdate(float elapsedTime) {
    previousScreenPosition = getScreenPosition();
    if (deleteOnLeaveScreen && !isPositionedWithinScreen()) {
        markForDeletion();
    } else {
        doOnUpdate(elapsedTime);
        updateMovement(elapsedTime);
        this->updateAnimation(elapsedTime);
    }
    animateRgb(elapsedTime);
}

void AnimatedGraphic::animateRgb(float elapsedTime) {
    if (MathUtils::floatEquals(requestedRgbAnimationTime, -1.0f)) {
        return;
    }
    totalRgbAnimationTime += elapsedTime;
    float remainingSeconds = requestedRgbAnimationTime - totalRgbAnimationTime;
    if ((targetRgb.r > originalRgb.r && rgb.r >= targetRgb.r)
        || (originalRgb.r > targetRgb.r && rgb.r <= targetRgb.r)
        || remainingSeconds <= 0.0f) {
        std::swap(targetRgb, originalRgb);
        totalRgbAnimationTime = 0.0f;
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
    if (activeAnimation == nullptr) {
        return nullptr;
    }
    return activeAnimation->getActiveDrawable();
}

void AnimatedGraphic::setCurrentAnimation(std::string animationId) {
    if (activeAnimation == nullptr) {
        std::cout << "Cannot switch to animation '" << animationId << "' because there is no active animation yet" << std::endl;
        return;
    }
    if (activeAnimation->getId().compare(animationId) != 0) {
        activeAnimation = animationMap->find(animationId)->second;
        updateCurrentDrawableData();
    }
}

std::string AnimatedGraphic::getCurrentAnimationId() {
    if (activeAnimation == nullptr) {
        return "";
    }
    return activeAnimation->getId();
}

void AnimatedGraphic::setZIndex(uint16_t zIndex) {
    this->zIndex = zIndex;
}

uint16_t AnimatedGraphic::getZIndex() {
    return zIndex;
}

bool AnimatedGraphic::canCollideWith(AnimatedGraphic *otherGraphic, float elapsedTime) {
    if (getActiveDrawable() == nullptr || otherGraphic->getActiveDrawable() == nullptr) {
        return false;
    }
    
    if ((getPreviousScreenPositionRight() < otherGraphic->getPreviousScreenPositionLeft() && getScreenPositionRight() < otherGraphic->getScreenPositionLeft())     ||
        (getPreviousScreenPositionLeft() > otherGraphic->getPreviousScreenPositionRight() && getScreenPositionLeft() > otherGraphic->getScreenPositionRight()) ||
        (getPreviousScreenPositionBottom() < otherGraphic->getPreviousScreenPositionTop() && getScreenPositionBottom() < otherGraphic->getScreenPositionTop()) ||
        (getPreviousScreenPositionTop() > otherGraphic->getPreviousScreenPositionBottom() && getScreenPositionTop() > otherGraphic->getScreenPositionBottom())) {
        hasNoCollisionWith(otherGraphic);
        otherGraphic->hasNoCollisionWith(this);
        return false;
    }
    return true;
}

void AnimatedGraphic::checkCollisionRect(AnimatedGraphic* otherGraphic, float elapsedTime) {
    if ((MathUtils::smallerOrEquals(getPreviousScreenPositionBottom(), otherGraphic->getPreviousScreenPositionTop()) && getScreenPositionBottom() > otherGraphic->getScreenPositionTop())
            && !MathUtils::floatEquals(getScreenPositionRight(), otherGraphic->getScreenPositionLeft()) && !MathUtils::floatEquals(getScreenPositionLeft(), otherGraphic->getScreenPositionRight())) {
        hasCollidedRect(otherGraphic, Geo2D::Side::BOTTOM);
        hasCollidedRectBottom(otherGraphic);
    } else if ((MathUtils::largerOrEquals(getPreviousScreenPositionTop(), otherGraphic->getPreviousScreenPositionBottom()) && getScreenPositionTop() < otherGraphic->getScreenPositionBottom())
            && !MathUtils::floatEquals(getScreenPositionRight(), otherGraphic->getScreenPositionLeft()) && !MathUtils::floatEquals(getScreenPositionLeft(), otherGraphic->getScreenPositionRight())) {
        hasCollidedRect(otherGraphic, Geo2D::Side::TOP);
        hasCollidedRectTop(otherGraphic);
    } else if ((MathUtils::largerOrEquals(getPreviousScreenPositionLeft(), otherGraphic->getPreviousScreenPositionRight()) && getScreenPositionLeft() < otherGraphic->getScreenPositionRight())
               && !MathUtils::floatEquals(getScreenPositionBottom(), otherGraphic->getScreenPositionTop()) && !MathUtils::floatEquals(getScreenPositionTop(), otherGraphic->getScreenPositionBottom())) {
        hasCollidedRect(otherGraphic, Geo2D::Side::LEFT);
        hasCollidedRectLeft(otherGraphic);
    } else if ((MathUtils::smallerOrEquals(getPreviousScreenPositionRight(), otherGraphic->getPreviousScreenPositionLeft()) && getScreenPositionRight() > otherGraphic->getScreenPositionLeft())
            && !MathUtils::floatEquals(getScreenPositionBottom(), otherGraphic->getScreenPositionTop()) && !MathUtils::floatEquals(getScreenPositionTop(), otherGraphic->getScreenPositionBottom())) {
        hasCollidedRect(otherGraphic, Geo2D::Side::RIGHT);
        hasCollidedRectRight(otherGraphic);
    } else {
        hasNoCollisionWith(otherGraphic);
        otherGraphic->hasNoCollisionWith(this);
    }
}

void AnimatedGraphic::hasNoCollisionWith(AnimatedGraphic* otherGraphic) {
    if (stayOnTopOfGraphic == otherGraphic) {
        stayOnTopOfGraphic = nullptr;
        moveVelocity.y -= otherGraphic->moveVelocity.y;
    }
    onHasNoCollisionWith(otherGraphic);
}

bool AnimatedGraphic::isPositionedWithinScreen() {
    return getActiveDrawable()->isPositionedWithinScreen();
}

void AnimatedGraphic::updateAnimation(float elapsedTime) {
    elapsedTimeSinceLastSwap += elapsedTime;
    if (activeAnimation != nullptr) {
        if (activeAnimation->getSpriteSwapInterval() >= 0 && elapsedTimeSinceLastSwap >= activeAnimation->getSpriteSwapInterval()) {
            activeAnimation->switchToNextDrawable();
            elapsedTimeSinceLastSwap = 0;
        }
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

void AnimatedGraphic::setX(float x) {
    position.x = x;
}

void AnimatedGraphic::setY(float y) {
    position.y = y;
}

float AnimatedGraphic::getTransparency() {
    return transparency;
}

void AnimatedGraphic::setTransparency(float transparency) {
    this->transparency = transparency;
}

Vector2D& AnimatedGraphic::getPosition() {
    return position;
}

Vector2D AnimatedGraphic::getPreviousScreenPosition() {
    if (previousScreenPosition.isEmpty()) {
        previousScreenPosition = getScreenPosition();
    }
    return previousScreenPosition;
}

Vector2D AnimatedGraphic::getScreenPosition() {
    return position + (applyScrolling ? ScrollingWorld::getInstance()->getOffset() : Vector2D::empty());
}

Vector2D& AnimatedGraphic::getMoveVelocity() {
    return moveVelocity;
}

Vector2D AnimatedGraphic::getVelocity() {
    return moveVelocity + gravityVelocity;
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
    Drawable* drawable = getActiveDrawable();
    if (drawable == nullptr) {
        return 0.0f;
    }
    return drawable->getSingleWidth();
}

int AnimatedGraphic::getWidth() {
    Drawable* drawable = getActiveDrawable();
    if (drawable == nullptr) {
        return 0.0f;
    }
    return drawable->getWidth();
}

int AnimatedGraphic::getHeight() {
    Drawable* drawable = getActiveDrawable();
    if (drawable == nullptr) {
        return 0.0f;
    }
    return drawable->getHeight();
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
    return getScreenPosition().x + getMarginLeft();
}

float AnimatedGraphic::getPreviousScreenPositionTop() {
    return getPreviousScreenPosition().y + getMarginTop();
}

float AnimatedGraphic::getPreviousScreenPositionBottom() {
    return getPreviousScreenPosition().y + getHeight() - getMarginBottom();
}

float AnimatedGraphic::getPreviousScreenPositionRight() {
    return getPreviousScreenPosition().x + getWidth() - getMarginRight();
}

float AnimatedGraphic::getPreviousScreenPositionLeft() {
    return getPreviousScreenPosition().x + getMarginLeft();
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

void AnimatedGraphic::move(Vector2D targetPoint, float pixelsPerSecond) {
    moveVelocity = jimp::Geo2D::vectorFrom(getPosition(), targetPoint, pixelsPerSecond);
}

void AnimatedGraphic::moveBetweenPoints(Vector2D startPoint, Vector2D targetPoint, float pixelsPerSecond) {
    moveVelocity = jimp::Geo2D::vectorFrom(getPosition(), targetPoint, pixelsPerSecond); 
}

void AnimatedGraphic::jump(float force) {
    gravityVelocity.y = -force;
}

void AnimatedGraphic::stopMoving() {
    moveVelocity.x = 0;
    moveVelocity.y = 0;
}

void AnimatedGraphic::interruptJump() {
    if (gravityVelocity.y < 0) {
        gravityVelocity.y = 0;
    }
}

void AnimatedGraphic::resetGravityVelocity() {
    gravityVelocity.y = 0.0f;
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

void AnimatedGraphic::animateRgbLevels(Color to, float seconds) {
    this->requestedRgbAnimationTime = seconds;
    targetRgb = to;
    originalRgb = rgb;
    totalRgbAnimationTime = 0.0f;
}

void AnimatedGraphic::stayOnTopOf(AnimatedGraphic *otherGraphic) {
    float otherGraphicPositionTop = applyScrolling ? otherGraphic->getWorldPositionTop() : otherGraphic->getScreenPositionTop();
    getPosition().y = otherGraphicPositionTop - (getHeight() - getMarginBottom());
    stayOnTopOfGraphic = otherGraphic;
    resetGravityVelocity();
}

void AnimatedGraphic::stayToLeftOf(AnimatedGraphic *otherGraphic) {
    float otherGraphicPositionLeft = applyScrolling ? otherGraphic->getWorldPositionLeft() : otherGraphic->getScreenPositionLeft();
    getPosition().x = otherGraphicPositionLeft - (getWidth() - getMarginRight());
}

void AnimatedGraphic::stayToRightOf(AnimatedGraphic *otherGraphic) {
    float otherGraphicPositionRight = applyScrolling ? otherGraphic->getWorldPositionRight() : otherGraphic->getScreenPositionRight();
    getPosition().x = otherGraphicPositionRight - getMarginLeft();
}

void AnimatedGraphic::updateMovement(float elapsedTime) {
    if (stayOnTopOfGraphic != nullptr) {
        moveVelocity.y = stayOnTopOfGraphic->moveVelocity.y;
    }
    if (!applyGravity && getVelocity().x == 0 && getVelocity().y == 0) {
          return;
    }
    if (applyGravity)  {
        gravityVelocity.y += jimp::Timing::toValueForElapsedTime(GameEngine::getInstance()->getGravityForce(), elapsedTime);
    }
    addToPosition(jimp::Timing::toValueForElapsedTime(gravityVelocity, elapsedTime));
    addToPosition(jimp::Timing::toValueForElapsedTime(moveVelocity, elapsedTime));
    
}

void AnimatedGraphic::updateCurrentDrawableData() {
    Drawable* activeDrawable = getActiveDrawable();
    if (activeDrawable == nullptr) {
        return;
    }
    activeDrawable->setPosition(getScreenPosition());
    activeDrawable->setScale(scale);
    activeDrawable->setRotationAngle(angle);
    activeDrawable->setTransparency(transparency);
    activeDrawable->setRotationPoint(getRotationPoint());
    Sprite* sprite = dynamic_cast<Sprite*>(activeDrawable);
    if (sprite != nullptr) {
        sprite->setDrawInversedHorizontally(inversedHorizontally);
        sprite->setRgbLevels(rgb);
    }
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

void AnimatedGraphic::lockForDeletion() {
    deleteLock->lock();
}

void AnimatedGraphic::unlockForDeletion() {
    deleteLock->unlock();
}

void AnimatedGraphic::setSpriteSwapInterval(std::string animationId, float swapInterval) {
    Animation* animation = nullptr;
    if (animationMap->find(animationId) != animationMap->end()) {
        animation = animationMap->find(animationId)->second;
    } else {
        animation = new Animation(animationId);
        animationMap->insert({ animationId, animation });
    }
    animation->setSpriteSwapInterval(swapInterval);
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
    }
}

}
