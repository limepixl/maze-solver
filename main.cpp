#include <SFML/Graphics.hpp>
#include <string>
#include "maze.h"
#include "button.h"

int main()
{
    srand((unsigned int)time(nullptr));   // Seed random number generation

    const int WIDTH = 800;
    const int HEIGHT = WIDTH + 200;

    int mazeSize;
    printf("Enter maze size: ");
    if(scanf("%d", &mazeSize) == 0)
    {
        printf("Failed to read maze size!\n");
        return -1;
    }
    mazeSize += !(mazeSize % 2);
    printf("Actual maze size (odd number): %d\n", mazeSize);

    // Window creation
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Mazes", sf::Style::Close);
    window.setFramerateLimit(30);

    Maze currentMaze(mazeSize);    // TEMP
       
    sf::Image mazeImage;
    mazeImage.create(mazeSize, mazeSize, sf::Color::Red);

    sf::Image test;
    sf::Texture mazeTexture;
    mazeTexture.loadFromImage(mazeImage);

    sf::Sprite mazeSprite(mazeTexture);
    mazeSprite.scale((float)WIDTH / mazeSize, (float)WIDTH / mazeSize);

    // Path depends on executable path
    sf::Font labelFont;
    labelFont.loadFromFile("../maze-solver/source-sans-pro/SourceSansPro-Regular.otf");
    float padding = 5.0f;

    int buttonCount = 4;
    Button buttons[buttonCount];
    buttons[0] = Button("Generate maze (Prim's)", labelFont, sf::Vector2f(0.0f, WIDTH), sf::Vector2f((float)WIDTH*0.5f, 50.0f), padding);
    buttons[0].action_generate = &RandomizedPrims;
    buttons[1] = Button("Solve maze", labelFont, sf::Vector2f((float)WIDTH*0.5f, WIDTH), sf::Vector2f((float)WIDTH*0.5f, 50.0f), padding);
    buttons[1].action_solve = &DepthFirstSearch;
    buttons[2] = Button("Change generation algorithm", labelFont, sf::Vector2f(0.0f, WIDTH + 50.0f), sf::Vector2f((float)WIDTH*0.5f, 50.0f), padding);
    buttons[2].action_change_gen = &CycleGenAlgorithm;

    while(window.isOpen())
    {
        sf::Event e;
        while(window.pollEvent(e))
        {
            if(e.type == sf::Event::Closed)
                window.close();
            if(e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                for(auto& b : buttons)
                {
                    sf::Vector2f origin = b.background.getPosition();
                    sf::Vector2f end = b.background.getSize();

                    // AABB collision
                    if(mousePos.x > origin.x && mousePos.x < origin.x + end.x && mousePos.y > origin.y && mousePos.y < origin.y + end.y)
                    {
                        printf("%s\n", b.label.getString().toAnsiString().c_str()); // Debug text
                        if(b.action_generate != nullptr)
                            currentMaze = b.action_generate(mazeSize, mazeImage, window);
                        else if(b.action_solve != nullptr)
                            b.action_solve(currentMaze, mazeImage, window);
                        else if(b.action_change_gen != nullptr)
                            b.action_change_gen(buttons, buttonCount);

                        mazeTexture.loadFromImage(mazeImage);
                        sf::Sprite temp(mazeTexture);
                        temp.setScale((float)WIDTH / mazeSize, (float)WIDTH / mazeSize);
                        mazeSprite = temp;
                    }
                }
            }
        }

        window.clear(sf::Color::Blue);

        for(int i = 0; i < buttonCount; i++)
        {
            window.draw(buttons[i].background);
            window.draw(buttons[i].label);
        }

        window.draw(mazeSprite);
        window.display();
    }
    
    return 0;
}
