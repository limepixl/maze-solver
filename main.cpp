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
    
    //Cell** solution = nullptr;
    
    bool isFinished = false;
    while(window.isOpen())
    {
        sf::Event e;
        while(window.pollEvent(e))
        {
            if(e.type == sf::Event::Closed)
                window.close();
        }
        
        window.clear(sf::Color::Blue);
        
        if(!isFinished)
        {
            isFinished = maze.Iterate(mazeImage);
            
            // Store image in texture (transition to sprite)            
            tex.loadFromImage(mazeImage);
            
            // Create sprite from texture
            mazeSprite = sf::Sprite(tex);
            mazeSprite.scale(scaleRatio, scaleRatio);    
            
            if(isFinished)
            {
                mazeImage.saveToFile("unsolved.png");
                printf("Finished generating maze!\n");
            } 
            /*
            if(solution == nullptr && isFinished)
            {
                solution = DepthFirstSearch(maze, mazeSize, &maze[startX], &maze[endX + mazeSize*mazeSize - mazeSize]);
                
                // Color in the solution path
                for(int i = 0; solution[i] != nullptr; i++)
                    mazeImage.setPixel(solution[i]->x, solution[i]->y, sf::Color::Red);
                
                // Store image in texture (transition to sprite)            
                tex.loadFromImage(mazeImage);
                
                // Create sprite from texture
                mazeSprite = sf::Sprite(tex);
                mazeSprite.scale(scaleRatio, scaleRatio); 
                mazeImage.saveToFile("solved.png");
                printf("Finished solving maze!\n");
            }
            */
        }    
        
        window.draw(mazeSprite);
        window.display();
    }
    
    return 0;
}
