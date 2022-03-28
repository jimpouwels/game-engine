#include "movingPlatform.hpp"
#include "gameEngine.hpp"

namespace ghosty {

MovingPlatform::MovingPlatform(jimp::Vector2D startPosition, jimp::Vector2D targetPosition) {
    this->startPosition = startPosition;
    this->targetPosition = targetPosition;
    this->setPosition(startPosition);
    moveBetweenPoints(startPosition, targetPosition, 200);
}

void MovingPlatform::doOnFrame(float elapsedTime) {
    jimp::Vector2D posTextPosition = jimp::Vector2D::from(getScreenPosition().x, getScreenPosition().y);
    posTextPosition.y -= 100;
    std::string text = "WorldPosition: " + std::to_string(getPosition().x) + ", " + std::to_string(getPosition().y);
    text += ("\nScreenPosition: " + std::to_string(getScreenPosition().x) + ", " + std::to_string(getScreenPosition().y));
    text += "\n";
    text += "Velocity: " + std::to_string(getVelocity().x) + ", " + std::to_string(getVelocity().y);
    jimp::GameEngine::getInstance()->draw(text , posTextPosition);
}

void MovingPlatform::doOnUpdate(float elapsedTime) {
    if (getPosition().distanceTo(startPosition) > targetPosition.distanceTo(startPosition)) {
        move(startPosition, 200);
    } else if (getPosition().distanceTo(targetPosition) > startPosition.distanceTo(targetPosition)) {
        move(targetPosition, 200);
    }
}

}
