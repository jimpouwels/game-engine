#include <SFML/Graphics.hpp>
#include <chrono>
#include <math.h>

#include "gameEngine.hpp"
#include "sprite.hpp"
#include "renderingCache.hpp"
#include "animatedGraphic.hpp"
#include "rectangle.hpp"
#include "keyboardHandler.hpp"

namespace jimp {

GameEngine::GameEngine(uint16_t screenWidth, uint16_t screenHeight, float gravityForce, std::string windowTitle, uint16_t desiredFrameRate, bool editMode) {
    this->editMode = editMode;
    instance = this;
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    this->gravityForce = gravityForce;
    this->frameRate = desiredFrameRate;
    this->timePerFrame = 1.0 / desiredFrameRate;
    this->windowTitle = windowTitle;
    auto fp1 = std::bind(&GameEngine::triggerUpdate, this, std::placeholders::_1);
    auto fp2 = std::bind(&GameEngine::handleDrawableDeleted, this, std::placeholders::_1);
    this->updateThread = new UpdateThread(fp1, fp2);
    this->renderCache = new RenderingCache();
    this->imageCache = new std::map<std::string, Image*>;
    this->soundCache = new std::map<std::string, Sound*>;
    window = new sf::RenderWindow(sf::VideoMode(this->getScreenWidth(), this->getScreenHeight()), windowTitle);
    this->previousFrameTime = std::chrono::system_clock::now();
    keyboardHandler = new jimp::KeyboardHandler();
    reloadLock = new std::mutex();
    reloadThread = new std::thread(&GameEngine::handleReloadStageRequest, this);
    new ScrollingWorld(10000, 3000);
}

GameEngine::~GameEngine() {
    reloadThread->join();
    delete reloadThread;
    updateThread->stop();
    delete updateThread;
    delete window;
    delete keyboardHandler;
    delete renderCache;
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
    reloadLock->lock();
}

bool GameEngine::isRunning() {
    return window->isOpen();
}

void GameEngine::loadStage(std::string filePath) {
    updateThread->pause();
    updateThread->removeAllGraphics();
    if (stageFactory == nullptr) {
        stageFactory = getStageFactory();
    }
    stageFactory->loadStage(filePath);
    onStageLoad(filePath);
    currentStage = filePath;
    updateThread->unpause();
}

bool GameEngine::isEditMode() {
    return editMode;
}

void GameEngine::drawFrame(float elapsedTimeSincePreviousFrame) {
    window->clear(sf::Color((backgroundColor << 8) + 0xFF));
    
    onFrame(elapsedTimeSincePreviousFrame);
    
    scrollingWorld->doOnFrame();
    updateThread->lockDeletionOfGraphics();
    for (const auto& graphic: *updateThread->getAllGraphics()) {
        if (!graphic->isMarkedForDeletion()) {
            graphic->onFrame(elapsedTimeSincePreviousFrame);
            draw(graphic->getActiveDrawable());
        }
    }
    updateThread->unlockDeletionOfGraphics();
    
    std::string statusText = "";
    if (isEditMode() && reloadingStage) {
        statusText = "Reloading...";
    } else if (isEditMode()) {
        statusText = "[Edit Mode]";
    }
    sf::Font font;
    font.loadFromFile("arial.ttf");
    sf::Text text = sf::Text();
    text.setFont(font);
    text.setString(statusText);
    text.setPosition(10, 10);
    window->draw(text);
    
    window->display();
}

void GameEngine::draw(Drawable* drawable) {
    if (drawable == nullptr || !drawable->isPositionedWithinScreen()) {
        return;
    }
    if (dynamic_cast<Sprite*>(drawable) != nullptr) {
        Sprite* sprite = dynamic_cast<Sprite*>(drawable);
        RenderingCache::CachedSprite* cachedSprite = nullptr;
        if (!renderCache->hasSprite(sprite->getFilePath())) {
            sf::Sprite* sfmlSprite = new sf::Sprite();
            sf::Texture* sfmlTexture = new sf::Texture();
            sfmlTexture->loadFromImage(sprite->getImage().getImage());
            sfmlSprite->setTexture(*sfmlTexture);
            
            cachedSprite = new RenderingCache::CachedSprite{ .texture = sfmlTexture, .sprite = sfmlSprite };
            renderCache->add(sprite->getFilePath(), cachedSprite);
        } else {
            cachedSprite = renderCache->getSprite(sprite->getFilePath());
        }

        for (uint16_t i = 0; i < sprite->getRepeat(); i++) {
            float offsetX = sprite->getPosition().x + i * sprite->getSingleWidth();
            sf::Transform transform;
            transform.rotate(sprite->getRotationAngle(), sprite->getPosition().x + sprite->getRotationPoint().x, sprite->getPosition().y + sprite->getRotationPoint().y);
            cachedSprite->sprite->setPosition(offsetX, sprite->getPosition().y);
            cachedSprite->sprite->setScale(sprite->getScale(), sprite->getScale());
            cachedSprite->sprite->setColor(sf::Color(fmin(255.0, floor(sprite->getRgbLevels().r)), fmin(255.0, floor(sprite->getRgbLevels().g)), fmin(255.0, floor(sprite->getRgbLevels().b)), fmin(100, sprite->getTransparency()) * (255.0f / 100.0f)));
            if (sprite->drawInversedHorizontally()) {
                cachedSprite->sprite->setTextureRect(sf::IntRect(cachedSprite->sprite->getTexture()->getSize().x, 0, -(cachedSprite->sprite->getTexture()->getSize().x), cachedSprite->sprite->getTexture()->getSize().y));
            } else {
                cachedSprite->sprite->setTextureRect(sf::IntRect(0, 0, cachedSprite->sprite->getTexture()->getSize().x, cachedSprite->sprite->getTexture()->getSize().y));
            }
            window->draw(*cachedSprite->sprite, transform);
        }
    } else if (dynamic_cast<Rectangle*>(drawable) != nullptr) {
        Rectangle* rectangle = dynamic_cast<Rectangle*>(drawable);
        drawRectangle(rectangle->getWidth(), rectangle->getHeight(), rectangle->getPosition(), rectangle->getColor());
    }
}

void GameEngine::drawRectangle(float width, float height, Vector2D position, Color color) {
    sf::RectangleShape shape;
    shape.setSize(sf::Vector2f(width, height));
    shape.setPosition(position.x, position.y);
    shape.setFillColor(sf::Color(color.r, color.g , color.b, 255));
    window->draw(shape);
}

void GameEngine::registerGraphic(AnimatedGraphic *graphic) {
    addKeyListener(graphic);
    updateThread->registerGraphic(graphic);
}

Image* GameEngine::createNewImage(std::string filePath) {
    Image* image = nullptr;
    if (imageCache->find(filePath) == imageCache->end()) {
        image = new Image(filePath);
        imageCache->insert({filePath, image});
    } else {
        image = imageCache->find(filePath)->second;
    }
    return image;
}

Sound* GameEngine::createNewSound(std::string filePath) {
    Sound* sound = nullptr;
    if (soundCache->find(filePath) == soundCache->end()) {
        sound = new Sound(filePath);
        soundCache->insert({filePath, sound});
    } else {
        sound = soundCache->find(filePath)->second;
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

float GameEngine::getGravityForce() {
    return gravityForce;
}

bool GameEngine::isAtLeftEdgeOfScreen(AnimatedGraphic* sprite) {
    return sprite->getPosition().x < 0;
}

bool GameEngine::isAtRightEdgeOfScreen(AnimatedGraphic* sprite) {
    return sprite->getPosition().x > getScreenWidth() - sprite->getWidth();
}

bool GameEngine::isAtTopEdgeOfScreen(AnimatedGraphic* sprite) {
    return sprite->getPosition().y < 0;
}

bool GameEngine::isAtBottomEdgeOfScreen(AnimatedGraphic* sprite) {
    return sprite->getPosition().y > getScreenHeight() - sprite->getHeight();
}

bool GameEngine::isOutsideScreenTop(AnimatedGraphic* sprite) {
    return sprite->getPosition().y < -sprite->getHeight();
}

bool GameEngine::isOutsideScreenBottom(AnimatedGraphic* sprite) {
    return sprite->getPosition().y > getScreenHeight();
}

bool GameEngine::isOutsideScreenLeft(AnimatedGraphic* sprite) {
    return sprite->getPosition().x < -(sprite->getWidth());
}

bool GameEngine::isOutsideScreenRight(AnimatedGraphic* sprite) {
    return sprite->getPosition().x > getScreenWidth();
}

std::vector<AnimatedGraphic*>* GameEngine::getAllGraphics() {
    return updateThread->getAllGraphics();
}

void GameEngine::setBackgroundColor(uint32_t color) {
    backgroundColor = color;
}

void GameEngine::reloadCurrentStage() {
    reloadRequested = true;
}

void GameEngine::handleReloadStageRequest() {
    while (window->isOpen() && isEditMode()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        if (reloadLock->try_lock()) {
            if (reloadRequested && !reloadingStage) {
                stageFactory->stopCurrentLoadingAction();
                reloadingStage = true;
                loadStage(currentStage);
                reloadingStage = false;
            }
            reloadRequested = false;
        }
        reloadLock->unlock();
    }
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

void GameEngine::handleDrawableDeleted(AnimatedGraphic* graphic) {
    onGraphicDeleted(graphic);
    keyboardHandler->removeKeyListener(graphic);
}

void GameEngine::handleSounds(float elapsedTime) {
    for (auto const& sound : *soundCache) {
        sound.second->doOnUpdate(elapsedTime);
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
