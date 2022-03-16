#ifndef scrollingWorld_hpp
#define scrollingWorld_hpp

#include "animatedGraphic.hpp"
#include "keyListener.hpp"

namespace jimp {

class GameEngine;

class ScrollingWorld : public KeyListener {
    
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
    
public:
    ScrollingWorld(int width, int height);
    ~ScrollingWorld();
    int getWidth();
    int getHeight();
    Vector2D getOffset();
    void doOnUpdate(float elapsedTime);
    void doOnFrame();
    AnimatedGraphic* getMainCharacter();
    void setMainCharacter(AnimatedGraphic* animatedGraphic);
    static ScrollingWorld* getInstance();
    void onKeyboardLeft(jimp::KeyState keyState) override;
    void onKeyboardRight(jimp::KeyState keyState) override;
    void onKeyboardUp(jimp::KeyState keyState) override;
    void onKeyboardDown(jimp::KeyState keyState) override;
};

}

#endif
