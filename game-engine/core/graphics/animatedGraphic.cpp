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
    
    float remainingDifferenceR = targetRgb.r - rgb.r;
    float remainingDifferenceG = targetRgb.g - rgb.g;
    float remainingDifferenceB = targetRgb.b - rgb.b;
    float rDelta = jimp::Timing::toValueForElapsedTime(remainingDifferenceR / remainingSeconds, elapsedTime);
    float gDelta = jimp::Timing::toValueForElapsedTime(remainingDifferenceG / remainingSeconds, elapsedTime);
    float bDelta = jimp::Timing::toValueForElapsedTime(remainingDifferenceB / remainingSeconds, elapsedTime);
    
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
    if (getName() == "ghosty" && otherGraphic->getName() == "DEZE") {
        std::cout << "velocity before collision check: " << getVelocity().y << std::endl;
    }
    if (currentGraphicCurrentPosition.x + getWidth() - getMarginRight() < otherGraphicCurrentPosition.x + otherGraphic->getMarginLeft()
        && currentGraphicNextPosition.x + getWidth() - getMarginRight() < otherGraphicNextPosition.x + otherGraphic->getMarginLeft()) {
        if (getName() == "ghosty" && otherGraphic->getName() == "DEZE") {
            std::cout << rand() << "   LINKS ERVAN" << std::endl;
        }
        return;
    }
    if (currentGraphicCurrentPosition.x + getMarginLeft() > otherGraphicCurrentPosition.x + otherGraphic->getWidth() - otherGraphic->getMarginRight()
        && currentGraphicNextPosition.x + getMarginLeft() > otherGraphicNextPosition.x + otherGraphic->getWidth() - otherGraphic->getMarginRight()) {
        if (getName() == "ghosty" && otherGraphic->getName() == "DEZE") {
            std::cout << rand() << "   RECHTS ERVAN" << std::endl;
        }
        return;
    }
    if (currentGraphicCurrentPosition.y + getHeight() - getMarginBottom() < otherGraphicCurrentPosition.y + otherGraphic->getMarginTop()
        && currentGraphicNextPosition.y + getHeight() - getMarginBottom() < otherGraphicNextPosition.y + otherGraphic->getMarginTop()) {
        if (getName() == "ghosty" && otherGraphic->getName() == "DEZE") {
            std::cout << rand() << "   ERBOVEN" << std::endl;
        }
        return;
    }
    if (currentGraphicCurrentPosition.y + getMarginTop() > otherGraphicCurrentPosition.y + otherGraphic->getHeight() - otherGraphic->getMarginBottom()
        && currentGraphicNextPosition.y + getMarginTop() > otherGraphicNextPosition.y + otherGraphic->getHeight() - otherGraphic->getMarginBottom()) {
        if (getName() == "ghosty" && otherGraphic->getName() == "DEZE") {
            std::cout << rand() << "   ERONDER" << std::endl;
        }
        return;
    }
    if (getName() == "ghosty" && otherGraphic->getName() == "DEZE") {
        std::cout << "ghosty current: " << currentGraphicCurrentPosition.x + getMarginLeft() << ", otherGraphicCurrent: " << otherGraphicCurrentPosition.x + otherGraphic->getWidth() - otherGraphic->getMarginRight() << ", currentGraphicNext: " << currentGraphicNextPosition.x + getMarginLeft() << ", otherGraphicNext: " << otherGraphicNextPosition.x + otherGraphic->getWidth() - otherGraphic->getMarginTop() << std::endl;
    }
    if (((currentGraphicCurrentPosition.y + getHeight() - getMarginBottom() < otherGraphicCurrentPosition.y + otherGraphic->getMarginTop()
          || MathUtils::floatEquals(currentGraphicCurrentPosition.y + getHeight() - getMarginBottom(), otherGraphicCurrentPosition.y + otherGraphic->getMarginTop()))
         && currentGraphicNextPosition.y + getHeight() - getMarginBottom() > otherGraphicNextPosition.y + otherGraphic->getMarginTop())
        && currentGraphicCurrentPosition.y != currentGraphicNextPosition.y) {
        if (getName() == "ghosty" && otherGraphic->getName() == "DEZE") {
            std::cout << rand() << "   BOTTOM HIT" << std::endl;
            std::cout << "ghosty.x: " << getPosition().x + getMarginLeft() << " block.x: " << otherGraphic->getPosition().x + otherGraphic->getWidth() - otherGraphic->getMarginRight() << ", ghostvel.x: " << getVelocity().x << std::endl;
        }
        hasCollidedRect(otherGraphic, Geo2D::Side::BOTTOM);
        hasCollidedRectBottom(otherGraphic);
    } else if (((currentGraphicCurrentPosition.y + getMarginTop() > otherGraphicCurrentPosition.y + otherGraphic->getHeight() - otherGraphic->getMarginBottom()
                 || MathUtils::floatEquals(currentGraphicCurrentPosition.y + getMarginTop(), otherGraphicCurrentPosition.y + otherGraphic->getHeight() - otherGraphic->getMarginBottom()))
                  && currentGraphicNextPosition.y + getMarginTop() < otherGraphicNextPosition.y + otherGraphic->getHeight() - otherGraphic->getMarginBottom())) {
        if (getName() == "ghosty" && otherGraphic->getName() == "DEZE") {
            std::cout << rand() << "   TOP HIT" << std::endl;
            std::cout << "ghosty.x: " << getPosition().x + getMarginLeft() << " block.x: " << otherGraphic->getPosition().x + otherGraphic->getWidth() - otherGraphic->getMarginRight() << ", ghostvel.x: " << getVelocity().x << std::endl;
        }
        hasCollidedRect(otherGraphic, Geo2D::Side::TOP);
        hasCollidedRectTop(otherGraphic);
    } else if (((currentGraphicCurrentPosition.x + getWidth() - getMarginRight() < otherGraphicCurrentPosition.x + otherGraphic->getMarginLeft()
                || MathUtils::floatEquals(currentGraphicCurrentPosition.x + getWidth() - getMarginRight(), otherGraphicCurrentPosition.x + otherGraphic->getMarginLeft()))
               && currentGraphicNextPosition.x + getWidth() - getMarginRight() > otherGraphicNextPosition.x + otherGraphic->getMarginLeft())) {
        if (getName() == "ghosty" && otherGraphic->getName() == "DEZE") {
            std::cout << rand() << "   BOTTOM HIT" << std::endl;
            std::cout << "ghosty.x: " << getPosition().x + getMarginLeft() << " block.x: " << otherGraphic->getPosition().x + otherGraphic->getWidth() - otherGraphic->getMarginRight() << ", ghostvel.x: " << getVelocity().x << std::endl;
        }
        hasCollidedRect(otherGraphic, Geo2D::Side::RIGHT);
        hasCollidedRectRight(otherGraphic);
    } else if (((currentGraphicCurrentPosition.x + getMarginLeft() > otherGraphicCurrentPosition.x + otherGraphic->getWidth() - otherGraphic->getMarginRight()
                 || MathUtils::floatEquals(currentGraphicCurrentPosition.x + getMarginLeft(), otherGraphicCurrentPosition.x + otherGraphic->getWidth() - otherGraphic->getMarginRight()))
                  && currentGraphicNextPosition.x + getMarginLeft() < otherGraphicNextPosition.x + otherGraphic->getWidth() - otherGraphic->getMarginRight())) {
        if (getName() == "ghosty" && otherGraphic->getName() == "DEZE") {
            std::cout << rand() << "   BOTTOM HIT" << std::endl;
            std::cout << "ghosty.x: " << getPosition().x + getMarginLeft() << " block.x: " << otherGraphic->getPosition().x + otherGraphic->getWidth() - otherGraphic->getMarginRight() << ", ghostvel.x: " << getVelocity().x << std::endl;
        }
        hasCollidedRect(otherGraphic, Geo2D::Side::LEFT);
        hasCollidedRectLeft(otherGraphic);
    } else {
        if (getName() == "ghosty" && otherGraphic->getName() == "DEZE") {
            std::cout << "END" << std::endl;
            std::cout << "ghosty.x: " << getPosition().x + getMarginLeft() << " block.y: " << otherGraphic->getPosition().x + otherGraphic->getWidth() - otherGraphic->getMarginRight() << ", ghostvel.x: " << getVelocity().x << std::endl;;
        }
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
    lock->lock();
    moveVelocity = jimp::Geo2D::vectorFrom(angle, pixelsPerSecond);
    lock->unlock();
}

void AnimatedGraphic::stopMoving() {
    lock->lock();
    moveVelocity.x = 0;
    moveVelocity.y = 0;
    lock->unlock();
}

void AnimatedGraphic::jump(float force) {
    lock->lock();
    gravityVelocity.y = -force;
    lock->unlock();
}

void AnimatedGraphic::interruptJump() {
    lock->lock();
    if (gravityVelocity.y < 0) {
        gravityVelocity.y = -gravityVelocity.y;
    }
    lock->unlock();
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
    lock->lock();
    getPosition().y = otherGraphic->getPosition().y - getHeight() + getMarginBottom() + otherGraphic->getMarginTop();
    resetGravityVelocity();
    interruptGravity = true;
    if (getName() == "ghosty" && otherGraphic->getName() == "DEZE") {
        std::cout << "reset position::: ghosty.x: " << getPosition().y + getMarginLeft() << ", platform.y: " << otherGraphic->getPosition().x + otherGraphic->getWidth() - otherGraphic->getMarginRight() << std::endl;
    }
    lock->unlock();
}

void AnimatedGraphic::stayToLeftOf(AnimatedGraphic *otherGraphic) {
    lock->lock();
    getPosition().x = otherGraphic->getPosition().x + otherGraphic->getMarginLeft() - (getHeight() - getMarginRight());
    interruptMovementX = true;
    lock->unlock();
}

void AnimatedGraphic::stayToRightOf(AnimatedGraphic *otherGraphic) {
    lock->lock();
    getPosition().x = otherGraphic->getPosition().x + otherGraphic->getWidth() - otherGraphic->getMarginRight() - getMarginLeft();
    interruptMovementX = true;
    lock->unlock();
}

void AnimatedGraphic::updateMovement(float elapsedTime) {
    if (applyGravity && !interruptGravity)  {
        gravityVelocity.y += jimp::Timing::toValueForElapsedTime(GameEngine::getInstance()->getGravityForce(), elapsedTime);
    }
    interruptGravity = false;
    
    addToPosition(jimp::Timing::toValueForElapsedTime(gravityVelocity, elapsedTime));
    
    if (interruptMovementX) {
        interruptMovementX = false;
        addToPosition(jimp::Timing::toValueForElapsedTime(Vector2D { .x = 0, .y = moveVelocity.y }, elapsedTime));
    }
    addToPosition(jimp::Timing::toValueForElapsedTime(moveVelocity, elapsedTime));
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
