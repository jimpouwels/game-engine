#ifndef spriteCache_h
#define spriteCache_h

#include <map>
#include <SFML/Graphics.hpp>

namespace jimp {

class RenderingCache {
    
public:
    struct CachedSprite {
        sf::Texture* texture;
        sf::Sprite* sprite;
        
        ~CachedSprite() {
            delete texture;
            delete sprite;
        }
    };
    
    RenderingCache() {
        spriteCache = new std::map<std::string, CachedSprite*>;
    }
    
    ~RenderingCache() {
        for (const auto& [sprite, cachedSprite]: *spriteCache) {
            delete cachedSprite;
        }
    }
    
    bool hasSprite(std::string sprite) {
        return !(spriteCache->find(sprite) == spriteCache->end());
    }
    
    void add(std::string sprite, CachedSprite* cachedSprite) {
        spriteCache->insert({sprite, cachedSprite});
    }
    
    void remove(std::string sprite) {
        if (hasSprite(sprite)) {
            spriteCache->erase(sprite);
        }
    }

    CachedSprite* getSprite(std::string sprite) {
        return spriteCache->find(sprite)->second;
    }
    
private:
    std::map<std::string, CachedSprite*>* spriteCache = nullptr;
    
};


}

#endif
