#include "gameEngine.hpp"
#include "sprite.hpp"
#include "graphic.hpp"
#include "gameEngine.hpp"
#include "vector2D.hpp"
#include "character.hpp"

class Mario : public jimp::GameEngine {
    
private:
    
public:
    Mario(int screenWidth, int screenHeight, std::string name) : GameEngine(screenWidth, screenHeight, name, 60) {
        new Character();
    }
    
    void startGame() {
        this->start();
    }
};

//int main() {
//    Mario game(1500, 900, "Mario");
//    game.startGame();
//    return 0;
//}
