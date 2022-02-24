#include <SFML/Graphics.hpp>
#include "rectangle.hpp"

namespace jimp {

sf::Shape Rectangle::getShape() {
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(getWidth(), getHeight()));
    rectangle.setOutlineColor(sf::Color::Red);
    rectangle.setOutlineThickness(5);
    rectangle.setPosition(getPosition().x, getPosition().y);
    rectangle.setFillColor(sf::Color::Green);
    rectangle.setOutlineColor(sf::Color::Green);
    return rectangle;
}

}
