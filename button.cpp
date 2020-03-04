#include "button.h"
#include "maze.h"

Button::Button(const std::string& text, sf::Font& font, const sf::Vector2f& pos, const sf::Vector2f& dimensions, float padding)
{
    label = sf::Text(text, font);
    label.setFillColor(sf::Color::White);
    label.setPosition(pos + sf::Vector2f(padding, padding));

    background = sf::RectangleShape(dimensions - sf::Vector2f(2*padding, 2*padding));
    background.setPosition(pos + sf::Vector2f(padding, padding));
    background.setFillColor(sf::Color(51,51,51,255));
}

void CycleGenAlgorithm(Button* buttons, int size)
{
    // Change generation algorithm on click
    // NOTE: Not using a map because I don't
    // plan to have many algorithms
    for(int i = 0; i < size; i++)
    {
        Button& b = buttons[i];
        if(b.action_generate != nullptr)
        {
            if(b.action_generate == &DepthFirstGen)
            {
                b.action_generate = &RandomizedPrims;
                b.label.setString(sf::String("Generate maze (Prim's)"));
            }
            else if(b.action_generate == &RandomizedPrims)
            {
                b.action_generate = &DepthFirstGen;
                b.label.setString(sf::String("Generate maze (Depth first)"));
            }

            break;
        }
    }
}
