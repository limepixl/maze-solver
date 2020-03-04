#include "button.h"

Button::Button(const std::string& text, sf::Font& font, const sf::Vector2f& pos, const sf::Vector2f& dimensions, float padding)
{
    label = sf::Text(text, font);
    label.setFillColor(sf::Color::White);
    label.setPosition(pos + sf::Vector2f(padding, padding));

    background = sf::RectangleShape(dimensions - sf::Vector2f(2*padding, 2*padding));
    background.setPosition(pos + sf::Vector2f(padding, padding));
    background.setFillColor(sf::Color(51,51,51,255));
}
