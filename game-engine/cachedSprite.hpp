#ifndef cachedSprite_hpp
#define cachedSprite_hpp

#include <SFML/Graphics.hpp>

namespace jimp {

class CachedSprite {
    
private:
    sf::Texture* texture;
    sf::Sprite* sprite;
    
public:
    CachedSprite(sf::Texture* texture, sf::Sprite* sprite);
    ~CachedSprite();
    sf::Sprite* getSprite();
    
};

}

#endif
