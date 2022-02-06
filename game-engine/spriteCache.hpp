#ifndef spriteCache_h
#define spriteCache_h

#include <map>
#include <SFML/Graphics.hpp>

namespace jimp {

class SpriteCache {
    
public:
    struct CachedSprite {
        sf::Texture* texture;
        sf::Sprite* sprite;
        
        ~CachedSprite() {
            delete texture;
            delete sprite;
        }
    };
    
    SpriteCache() {
        spriteCache = new std::map<Sprite*, CachedSprite*>;
    }
    
    bool hasSprite(Sprite* sprite) {
        return !(spriteCache->find(sprite) == spriteCache->end());
    }
    
    void add(Sprite* sprite, CachedSprite* cachedSprite) {
        spriteCache->insert({sprite, cachedSprite});
    }

    CachedSprite* getSprite(Sprite* sprite) {
        return spriteCache->find(sprite)->second;
    }
    
private:
    std::map<Sprite*, CachedSprite*>* spriteCache = nullptr;
    
};


}

#endif
