#include <SFML/Graphics.hpp>
#include "cell.h"

int main()
{
    const int WINDOW_SIZE = 800;
    
    int mazeSize;    
    printf("Maze size (odd number): ");
    scanf("%d", &mazeSize);
    float scaleRatio = (float)WINDOW_SIZE / mazeSize;
    
    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Mazes");
    Cell maze[mazeSize * mazeSize];
    
    int startX, endX;
    GenerateMaze(maze, mazeSize, startX, endX);
    int arrIndex = 0;
    Cell pathStack[mazeSize * mazeSize];
    
    // Starting cell is under the entrance
    Cell startCell(startX, 1);
    maze[startX + mazeSize].isWall = false;
    pathStack[arrIndex] = startCell;
    
    sf::Image mazeImage;
    sf::Texture tex;
    sf::Sprite mazeSprite;
    mazeImage.create(mazeSize, mazeSize, sf::Color::Red);
    
    Cell** solution = nullptr;
    
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
            isFinished = Iterate(startCell, maze, pathStack, arrIndex, mazeSize);
            
            // Create image from maze data
            for(int i = 0; i < mazeSize; i++)
            for(int j = 0; j < mazeSize; j++)
            {
                sf::Color pixelColor = (maze[j + i * mazeSize].isWall ? sf::Color::Black : sf::Color::White);
                mazeImage.setPixel(j, i, pixelColor);
            }
            
            // Store image in texture (transition to sprite)            
            tex.loadFromImage(mazeImage);
            
            // Create sprite from texture
            mazeSprite = sf::Sprite(tex);
            mazeSprite.scale(scaleRatio, scaleRatio);    
            
            if(isFinished)
            {
                const char* outputName = "output.png";
                mazeImage.saveToFile(outputName);
                printf("Finished generating maze! Output: %s\n", outputName);
            } 
            
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
            }
        }    
        
        window.draw(mazeSprite);
        window.display();
    }
    
    return 0;
}
