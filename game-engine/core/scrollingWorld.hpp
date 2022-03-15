#ifndef scrollingWorld_hpp
#define scrollingWorld_hpp

#include "animatedGraphic.hpp"
#include "keyListener.hpp"

namespace jimp {

class GameEngine;

class ScrollingWorld : public KeyListener {
    
private:
    AnimatedGraphic* mainCharacter = nullptr;
    bool mainCharacterLoaded = false;
    float offsetX = 0;
    float offsetY = 0;
    float maxScrollX = 0;
    float maxScrollY = 0;
    int width = 0;
    int height = 0;
    bool editMode = false;
    bool editCameraLeft = false;
    bool editCameraRight = false;
    bool editCameraUp = false;
    bool editCameraDown = false;
    
public:
    ScrollingWorld(int width, int height);
    ~ScrollingWorld();
    int getWidth();
    int getHeight();
    void doOnUpdate(float elapsedTime);
    void doOnFrame();
    void setEditMode(bool editMode);
    AnimatedGraphic* getMainCharacter();
    void setMainCharacterLoaded(bool loaded);
    void setMainCharacter(AnimatedGraphic* animatedGraphic);
    static ScrollingWorld* getInstance();
    void onKeyboardLeft(jimp::KeyState keyState) override;
    void onKeyboardRight(jimp::KeyState keyState) override;
    void onKeyboardUp(jimp::KeyState keyState) override;
    void onKeyboardDown(jimp::KeyState keyState) override;
};

}

#endif
