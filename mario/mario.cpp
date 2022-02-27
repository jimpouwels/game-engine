#include "gameEngine.hpp"
#include "sprite.hpp"
#include "graphic.hpp"
#include "gameEngine.hpp"
#include "vector2D.hpp"
#include "character.hpp"

class Mario : public jimp::GameEngine {
    
private:
    Character* character = nullptr;
    
public:
    Mario(int screenWidth, int screenHeight, std::string name) : GameEngine(screenWidth, screenHeight, name, 60) {
        character = new Character();
    }
    
    ~Mario() {
        delete character;
    }
    
    void startGame() {
        this->start();
    }
    
    void onFrame(float elapsedTime) {
        character->onFrame(elapsedTime);
    }
    
    void onUpdate(float elapsedTime) {
        character->onUpdate(elapsedTime);
    }
};

//int main() {
//    Mario game(1500, 900, "Mario");
//    game.startGame();
//    return 0;
//}
