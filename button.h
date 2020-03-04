#pragma once
#include <SFML/Graphics.hpp>
#include <string>

struct Button
{
    sf::RectangleShape background;
    sf::Text label;

    // Function pointer to the function that's called on click
    struct Maze (*action_generate)(int size, sf::Image& image, sf::RenderWindow& window) = nullptr;
    void (*action_solve)(struct Maze& maze, sf::Image& image, sf::RenderWindow& window) = nullptr;
    void (*action_change_gen)(Button* buttons, int size) = nullptr;

    Button() = default;
    Button(const std::string& text, sf::Font& font, const sf::Vector2f& pos, const sf::Vector2f& dimensions, float padding = 5.0f);
};

void CycleGenAlgorithm(Button* buttons, int size);
void CycleSolveAlgorithm(Button* buttons, int size);    // TODO
