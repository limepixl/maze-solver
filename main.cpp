#include <SFML/Graphics.hpp>
#include "maze.h"

int main()
{
    const int WINDOW_SIZE = 800;
    
    int mazeSize;    
    printf("Maze size (odd number): ");
    scanf("%d", &mazeSize);
    float scaleRatio = (float)WINDOW_SIZE / mazeSize;

    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Mazes");
    Maze maze(mazeSize);
       
    sf::Image mazeImage;
    sf::Texture tex;
    sf::Sprite mazeSprite;
    mazeImage.create(mazeSize, mazeSize, sf::Color::Red);
    
    bool finishedGenerating = false;
    bool finishedSolving = false;
    bool complete = false;
    while(window.isOpen())
    {
        sf::Event e;
        while(window.pollEvent(e))
        {
            if(e.type == sf::Event::Closed)
                window.close();
        }
        
        window.clear(sf::Color::Blue);
        
        if(!finishedGenerating)
        {
            finishedGenerating = maze.Iterate(mazeImage);
            
            // Store image in texture (transition to sprite)            
            tex.loadFromImage(mazeImage);
            
            // Create sprite from texture
            mazeSprite = sf::Sprite(tex);
            mazeSprite.scale(scaleRatio, scaleRatio);    
            
            if(finishedGenerating)
            {
                mazeImage.saveToFile("unsolved.png");
                printf("Finished generating maze!\n");
            }
        } else if(!finishedSolving)
        {
            finishedSolving = maze.DepthFirstSearch(mazeImage);

            // Store image in texture (transition to sprite)            
            tex.loadFromImage(mazeImage);
            
            // Create sprite from texture
            mazeSprite = sf::Sprite(tex);
            mazeSprite.scale(scaleRatio, scaleRatio); 
        } else if(!complete)
        {
            complete = true;
            mazeImage.saveToFile("solved.png");
            printf("Finished solving maze!\n");
        }
        
        window.draw(mazeSprite);
        window.display();
    }
    
    return 0;
}
