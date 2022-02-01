#include <vector>
#include <map>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "animatedSprite.hpp"
#include "sprite.hpp"
#include "animation.hpp"

namespace jimp {

AnimatedSprite::AnimatedSprite(Screen* screen, float x, float y, float scale, float imageSwapIntervalInSeconds) {
    this->position.x = x;
    this->position.y = y;
    this->scale = scale;
    this->screen = screen;
    animationMap = new std::map<std::string, Animation*>;
    this->imageSwapIntervalInSeconds = imageSwapIntervalInSeconds;
}

AnimatedSprite::AnimatedSprite(Screen* screen, float x, float y, float scale, int rotationAngle, float imageSwapIntervalInSeconds) {
    AnimatedSprite(screen, x, y, scale, 0, imageSwapIntervalInSeconds);
    this->angle = rotationAngle;
}

AnimatedSprite::~AnimatedSprite() {
    for (const auto& [animationId, animation]: *animationMap) {
        delete animation;
    }
    delete animationMap;
}

Sprite& AnimatedSprite::getActiveSprite() {
    Sprite* activeSprite = activeAnimation->getActiveSprite();
    activeSprite->setPosition(position);
    activeSprite->setScale(scale);
    activeSprite->setRotationAngle(angle);
    return *activeAnimation->getActiveSprite();
}

void AnimatedSprite::setCurrentAnimation(std::string animationId) {
    if (activeAnimation->getId().compare(animationId) != 0) {
        activeAnimation = animationMap->find(animationId)->second;
    }
}

void AnimatedSprite::updateAnimation(float elapsedTime) {
    elapsedTimeSinceLastSwap += elapsedTime;
    if (elapsedTimeSinceLastSwap >= imageSwapIntervalInSeconds) {
        activeAnimation->switchToNextSprite();
        elapsedTimeSinceLastSwap = 0;
    }
}

jimp::Screen& AnimatedSprite::getScreen() {
    return *screen;
}

void AnimatedSprite::setX(float x) {
    position.x = x;
}

void AnimatedSprite::setY(float y) {
    position.y = y;
}

Point2D& AnimatedSprite::getPosition() {
    return position;
}

void AnimatedSprite::setPosition(Point2D position) {
    this->position = position;
}

void AnimatedSprite::addToPosition(Point2D delta) {
    position.x += delta.x;
    position.y += delta.y;
}

int AnimatedSprite::getWidth() {
    return activeAnimation->getActiveSprite()->getWidth();
}

int AnimatedSprite::getHeight() {
    return activeAnimation->getActiveSprite()->getHeight();
}

float AnimatedSprite::getScale() {
    return scale;
}

bool AnimatedSprite::isPositionedWithinScreen() {
    return screen->isWithin(position.x, position.y);
    return true;
}

float AnimatedSprite::getRotationAngle() {
    return angle;
}

void AnimatedSprite::setRotationAngle(float angle) {
    this->angle = angle;
}

bool AnimatedSprite::isOutsideScreenLeft() {
    return getActiveSprite().isOutsideScreenLeft();
}

bool AnimatedSprite::isOutsideScreenRight() {
    return getActiveSprite().isOutsideScreenRight();
}

bool AnimatedSprite::isOutsideScreenAbove() {
    return getActiveSprite().isOutsideScreenAbove();
}

bool AnimatedSprite::isOutsideScreenBelow() {
    return getActiveSprite().isOutsideScreenBelow();
}

bool AnimatedSprite::isFacingUp() {
    return angle < 90 || angle > 270;
}

bool AnimatedSprite::isFacingDown() {
    return angle > 90 && angle < 270;
}

bool AnimatedSprite::isFacingLeft() {
    return angle > 180 && angle < 360;
}

bool AnimatedSprite::isFacingRight() {
    return angle > 0 && angle < 180;
}

void AnimatedSprite::addSprite(std::string animationId, std::string filePath) {
    Animation* animation = nullptr;
    if (animationMap->find(animationId) == animationMap->end()) {
        animation = new Animation(animationId);
        animationMap->insert({animationId, animation});
    } else {
        animation = animationMap->find(animationId)->second;
    }
    Sprite* sprite = new Sprite(screen, position.x, position.y, scale, filePath);
    animationMap->find(animationId)->second->addSprite(sprite);
    if (activeAnimation == nullptr) {
        activeAnimation = animation;
        updateAnimation(0);
    }
}

}
