#include <vector>
#include <map>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "animatedSprite.hpp"
#include "sprite.hpp"
#include "animation.hpp"

namespace jimp {

AnimatedSprite::AnimatedSprite(Screen* screen, float x, float y, float scale, float imageSwapIntervalInSeconds) {
    this->x = x;
    this->y = y;
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
    activeSprite->setPosition(x, y);
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

float AnimatedSprite::getX() {
    return x;
}

void AnimatedSprite::setX(float x) {
    this->x = x;
}

float AnimatedSprite::getY() {
    return y;
}

void AnimatedSprite::setY(float y) {
    this->y = y;
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
    return screen->isWithin(x, y);
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

void AnimatedSprite::addSprite(std::string animationId, std::string filePath) {
    Animation* animation = nullptr;
    if (animationMap->find(animationId) == animationMap->end()) {
        animation = new Animation(animationId);
        animationMap->insert({animationId, animation});
    } else {
        animation = animationMap->find(animationId)->second;
    }
    Sprite* sprite = new Sprite(screen, x, y, scale, filePath);
    animationMap->find(animationId)->second->addSprite(sprite);
    if (activeAnimation == nullptr) {
        activeAnimation = animation;
        updateAnimation(0);
    }
}

}
