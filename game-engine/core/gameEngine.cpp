#include <SFML/Graphics.hpp>
#include <chrono>

#include "gameEngine.hpp"
#include "sprite.hpp"
#include "spriteCache.hpp"
#include "graphic.hpp"
#include "keyboardHandler.hpp"

namespace jimp {

GameEngine::GameEngine(uint16_t screenWidth, uint16_t screenHeight, std::string windowTitle, uint16_t desiredFrameRate) {
    instance = this;
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    this->frameRate = desiredFrameRate;
    this->timePerFrame = 1.0 / desiredFrameRate;
    this->windowTitle = windowTitle;
    auto fp1 = std::bind(&GameEngine::triggerUpdate, this, std::placeholders::_1);
    auto fp2 = std::bind(&GameEngine::handleSpriteDeleted, this, std::placeholders::_1);
    this->updateThread = new UpdateThread(fp1, fp2);
    this->spriteCache = new SpriteCache();
    this->imageCache = new std::map<std::string, Image*>;
    this->soundCache = new std::map<std::string, Sound*>;
    window = new sf::RenderWindow(sf::VideoMode(this->getScreenWidth(), this->getScreenHeight()), windowTitle);
    keyboardHandler = new jimp::KeyboardHandler();
    this->previousFrameTime = std::chrono::system_clock::now();
}

GameEngine::~GameEngine() {
    delete updateThread;
    delete window;
    delete keyboardHandler;
    spriteCache->erase();
    delete spriteCache;
    for (const auto& [filePath, image]: *imageCache) {
        delete image;
    }
    delete imageCache;
    for (const auto& [filePath, sound]: *soundCache) {
        delete sound;
    }
    delete soundCache;
}

GameEngine* GameEngine::getInstance() {
    return instance;
}

void GameEngine::start() {
    updateThread->start();
    while (window->isOpen()) {
        handleEvents();
        
        if (isWindowClosed) {
            break;
        }
        
        std::chrono::time_point<std::chrono::system_clock> currentTime = std::chrono::system_clock::now();
        
        float elapsedTimeInMillis = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - previousFrameTime).count();
        float elapsedTimeInSeconds = elapsedTimeInMillis / 1000.0F;
        if (elapsedTimeInSeconds > timePerFrame || frameRate == -1) {
            totalFrames++;
            drawFrame(elapsedTimeInSeconds);
            handleSounds(elapsedTimeInSeconds);
            previousFrameTime = currentTime;
        }
        
        window->setTitle(windowTitle + " FPS: " + std::to_string(measureFps(currentTime)));
    }
    window->close();
}

void GameEngine::draw(jimp::Sprite* sprite) {
    SpriteCache::CachedSprite* cachedSprite = nullptr;
    if (!spriteCache->hasSprite(sprite)) {
        sf::Sprite* sfmlSprite = new sf::Sprite();
        sf::Texture* sfmlTexture = new sf::Texture();
        sfmlTexture->loadFromImage(sprite->getImage().getImage());
        sfmlSprite->setTexture(*sfmlTexture);
        
        cachedSprite = new SpriteCache::CachedSprite{ .texture = sfmlTexture, .sprite = sfmlSprite };
        spriteCache->add(sprite, cachedSprite);
    } else {
        cachedSprite = spriteCache->getSprite(sprite);
    }

    sf::Transform transform;
    transform.rotate(sprite->getRotationAngle(), sprite->getPosition().x + sprite->getRotationPoint().x, sprite->getPosition().y + sprite->getRotationPoint().y);
    cachedSprite->sprite->setPosition(sprite->getPosition().x, sprite->getPosition().y);
    cachedSprite->sprite->setScale(sprite->getScale(), sprite->getScale());
    window->draw(*cachedSprite->sprite, transform);
}

void GameEngine::registerGraphic(Graphic *graphic) {
    addKeyListener(graphic);
    updateThread->registerGraphic(graphic);
}

void GameEngine::drawRectangle(float width, float height, Vector2D position) {
    
    window->draw(rectangle);
}

Image* GameEngine::registerImage(Image* image) {
    if (imageCache->find(image->getFilePath()) == imageCache->end()) {
        imageCache->insert({image->getFilePath(), image});
    } else {
        image = imageCache->find(image->getFilePath())->second;
    }
    return image;
}

Sound* GameEngine::registerSound(Sound* sound) {
    if (soundCache->find(sound->getFilePath()) == soundCache->end()) {
        soundCache->insert({sound->getFilePath(), sound});
    } else {
        sound = soundCache->find(sound->getFilePath())->second;
    }
    return sound;
}

void GameEngine::addKeyListener(KeyListener* keyListener) {
    keyboardHandler->addKeyListener(keyListener);
}

int GameEngine::getScreenWidth() {
    return screenWidth;
}

int GameEngine::getScreenHeight() {
    return screenHeight;
}

bool GameEngine::isAtLeftEdgeOfScreen(Graphic* sprite) {
    return sprite->getPosition().x < 0;
}

bool GameEngine::isAtRightEdgeOfScreen(Graphic* sprite) {
    return sprite->getPosition().x > getScreenWidth() - sprite->getWidth();
}

bool GameEngine::isAtTopEdgeOfScreen(Graphic* sprite) {
    return sprite->getPosition().y < 0;
}

bool GameEngine::isAtBottomEdgeOfScreen(Graphic* sprite) {
    return sprite->getPosition().y > getScreenHeight() - sprite->getHeight();
}

bool GameEngine::isOutsideScreenTop(Graphic* sprite) {
    return sprite->getPosition().y < -sprite->getHeight();
}

bool GameEngine::isOutsideScreenBottom(Graphic* sprite) {
    return sprite->getPosition().y > getScreenHeight();
}

bool GameEngine::isOutsideScreenLeft(Graphic* sprite) {
    return sprite->getPosition().x < -(sprite->getWidth());
}

bool GameEngine::isOutsideScreenRight(Graphic* sprite) {
    return sprite->getPosition().x > getScreenWidth();
}

bool GameEngine::isPositionWithinScreen(Vector2D position) {
    return position.x <= getScreenWidth() && position.x >= 0 && position.y <= getScreenHeight() && position.y > 0;
}

void GameEngine::drawFrame(float elapsedTimeSincePreviousFrame) {
    window->clear();
    onFrame(elapsedTimeSincePreviousFrame);
    
    updateThread->lockForDeletion();
    for (const auto& sprite: *updateThread->getAllGraphics()) {
        if (!sprite->isMarkedForDeletion() && sprite->isInitialized()) {
            sprite->onFrame(elapsedTimeSincePreviousFrame);
            draw(sprite->getActiveSprite());
        }
    }
    
    updateThread->unlockForDeletion();
    window->display();
}

float GameEngine::measureFps(std::chrono::time_point<std::chrono::system_clock>& currentTime) {
    std::chrono::duration<float> elapsedTimeSincePreviousMeasurement = (currentTime - previousFpsMeasurementTime);
    if (elapsedTimeSincePreviousMeasurement.count() > 5) {
        previousFpsMeasurementTime = currentTime;
        totalFrames = 0;
    }
    return totalFrames / elapsedTimeSincePreviousMeasurement.count();
}

void GameEngine::triggerUpdate(float elapsedTime) {
    onUpdate(elapsedTime);
}

void GameEngine::handleSpriteDeleted(Graphic* graphic) {
    for (const auto& sprite: graphic->getAllSprites()) {
         spriteCache->remove(sprite);
    }
    onSpriteDeleted(graphic);
    keyboardHandler->removeKeyListener(graphic);
}

void GameEngine::handleSounds(float elapsedTime) {
    for (auto const& sound : *soundCache) {
        sound.second->onFrame(elapsedTime);
        sound.second->cleanupCompletedFullRunRuns();
    }
}

void GameEngine::handleEvents() {
    sf::Event event;
    
    while (window->pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                isWindowClosed = true;
                break;
            case sf::Event::KeyPressed:
                keyboardHandler->handleEvent(event);
                break;
            case sf::Event::KeyReleased:
                keyboardHandler->handleEvent(event);
                break;
        }
    }
}

}
