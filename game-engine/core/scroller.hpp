#ifndef scroller_hpp
#define scroller_hpp

#include "animatedGraphic.hpp"
#include "keyListener.hpp"

namespace jimp {

class GameEngine;

class Scroller : public KeyListener {
    
private:
    AnimatedGraphic* mainCharacter = nullptr;
    float offsetX = 0.0f;
    float offsetY = 0.0f;
    float maxScrollX = 0.0f;
    float maxScrollY = 0.0f;
    int width = 0;
    int height = 0;
    bool editCameraLeft = false;
    bool editCameraRight = false;
    bool editCameraUp = false;
    bool editCameraDown = false;
    float rightSideOfCamera = 0.0f;
    float leftSideOfCamera = 0.0f;
    float topSideOfCamera = 0.0f;
    float bottomSideOfCamera = 0.0f;
    
public:
    Scroller(int width, int height);
    ~Scroller();
    int getWidth();
    int getHeight();
    Vector2D getOffset();
    void doOnUpdate(float elapsedTime);
    void doOnFrame();
    AnimatedGraphic* getMainCharacter();
    void setMainCharacter(AnimatedGraphic* animatedGraphic);
    static Scroller* getInstance();
    void onKeyboardLeft(jimp::KeyState keyState) override;
    void onKeyboardRight(jimp::KeyState keyState) override;
    void onKeyboardUp(jimp::KeyState keyState) override;
    void onKeyboardDown(jimp::KeyState keyState) override;
};

}

#endif
