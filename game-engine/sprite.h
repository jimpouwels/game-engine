#ifndef GAME_ENGINE_SPRITE_H_
#define GAME_ENGINE_SPRITE_H_

namespace jimp {

    class Sprite {
        
    private:
        int x;
        int y;
        std::string filePath;
    public:
        Sprite(int x, int y, std::string filePath);
        int getX();
        int getY();
        std::string getFilePath();
        
    };

    Sprite::Sprite(int x, int y, std::string filePath) {
        this->x = x;
        this->y = y;
        this->filePath = filePath;
    }

    int Sprite::getX() {
        return this->x;
    }

    int Sprite::getY() {
        return this->y;
    }

    std::string Sprite::getFilePath() {
        return this->filePath;
    }
}

#endif
