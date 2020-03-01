#include <SFML/Graphics.hpp>
#include "maze.h"

int main()
{
    srand((unsigned int)time(nullptr));   // Seed random number generation

    const int WINDOW_SIZE = 800;
    
    int mazeSize;    
    printf("Maze size (odd number): ");
    if(scanf("%d", &mazeSize) == 0)
    {
        printf("Failed to read maze size!\n");
        return -1;
    }

    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Mazes", sf::Style::Close);
    Maze maze(mazeSize);
       
    sf::Image mazeImage;
    mazeImage.create(mazeSize, mazeSize, sf::Color::Red);

    // Generate maze
    maze.DepthFirstGen(mazeImage, window);

    // Save generated maze image
    mazeImage.saveToFile("unsolved.png");
    printf("Finished generating maze!\n");

    // Solve maze
    maze.DepthFirstSearch(mazeImage, window);

    // Save solved maze image
    mazeImage.saveToFile("solved.png");
    printf("Finished solving maze!\n");

    sf::Texture mazeTexture;
    mazeTexture.loadFromImage(mazeImage);

    sf::Sprite mazeSprite(mazeTexture);
    mazeSprite.scale((float)WINDOW_SIZE / mazeSize, (float)WINDOW_SIZE / mazeSize);

    while(window.isOpen())
    {
        sf::Event e;
        while(window.pollEvent(e))
        {
            if(e.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Blue);
        window.draw(mazeSprite);
        window.display();
    }
    
    return 0;
}
