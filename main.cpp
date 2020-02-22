#include <SFML/Graphics.hpp>

struct Cell
{
    int x, y;
    bool isWall;
    bool canBranchOut;
    
    Cell() = default;
    Cell(int x, int y) : x(x), y(y)
    {
        isWall = true;
        canBranchOut = true;
    }
};

int GenerateMaze(Cell* maze, int size)
{
    srand(time(nullptr));   // Seed random number generation
    
    int start = rand() % (size - 2) + 1;
    int end = rand() % (size - 2) + 1;

    // Make start and end even numbers
    start += (start % 2 != 0);
    end += (end % 2 != 0);
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {            
            maze[j + i*size] = Cell(j, i);
            
            if(i == 0 && j == start)
                maze[j + i*size].isWall = false;
            else if(i == size-1 && j == end)
                maze[j + i*size].isWall = false;
        }
    }
    
    return start;
}

bool Iterate(Cell& startCell, Cell* maze, Cell* pathStack, int& arrIndex, int size)
{
    int nCount = 0;
    Cell* neighbors[4];
    neighbors[0] = neighbors[1] = neighbors[2] = neighbors[3] = nullptr;
    
    if(startCell.x - 2 >= 1 && maze[startCell.x - 2 + startCell.y*size].isWall) // left
    {
        neighbors[0] = &maze[startCell.x - 2 + startCell.y*size];
        nCount++;
    }
    if(startCell.x + 2 <= size - 2 && maze[startCell.x + 2 + startCell.y*size].isWall) // right
    {
        neighbors[1] = &maze[startCell.x + 2 + startCell.y*size];
        nCount++;
    }
    if(startCell.y - 2 >= 1 && maze[startCell.x + (startCell.y - 2)*size].isWall) // up
    {
        neighbors[2] = &maze[startCell.x + (startCell.y - 2)*size];
        nCount++;
    }
    if(startCell.y + 2 <= size - 2 && maze[startCell.x + (startCell.y + 2)*size].isWall) // down
    {
        neighbors[3] = &maze[startCell.x + (startCell.y + 2)*size];
        nCount++;
    }
    
    // No neighbors
    if(nCount == 0)
    {
        startCell = pathStack[--arrIndex];
        
        if(arrIndex == 0)
            return true;
        
        return false;
    }
    
    int r = rand() % 4;
    while(neighbors[r] == nullptr || !neighbors[r]->isWall)
        r = rand() % 4;

    neighbors[r]->isWall = false;
    if(r == 0)
        maze[startCell.x - 1 + startCell.y*size].isWall = false;
    else if(r == 1)
        maze[startCell.x + 1 + startCell.y*size].isWall = false;
    else if(r == 2)
        maze[startCell.x + (startCell.y - 1)*size].isWall = false;
    else
        maze[startCell.x + (startCell.y + 1)*size].isWall = false;
    
    pathStack[++arrIndex] = *neighbors[r];
    startCell = pathStack[arrIndex];
    
    return false;
}

int main()
{
    const int WINDOW_SIZE = 800;
    const int MAZE_SIZE = 201;
    float scaleRatio = (float)WINDOW_SIZE / MAZE_SIZE;
    
    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Mazes");
    //window.setFramerateLimit(120);
    Cell maze[MAZE_SIZE * MAZE_SIZE];
    
    int startX = GenerateMaze(maze, MAZE_SIZE);
    int arrIndex = 0;
    Cell pathStack[MAZE_SIZE * MAZE_SIZE];
    
    // Starting cell is under the entrance
    Cell startCell(startX, 1);
    maze[startX + MAZE_SIZE].isWall = false;
    pathStack[arrIndex] = startCell;
    
    sf::Image mazeImage;
    sf::Texture tex;
    mazeImage.create(MAZE_SIZE, MAZE_SIZE, sf::Color::Red);
    
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
            isFinished = Iterate(startCell, maze, pathStack, arrIndex, MAZE_SIZE);
            
            // Create image from maze data
            for(int i = 0; i < MAZE_SIZE; i++)
            for(int j = 0; j < MAZE_SIZE; j++)
            {
                sf::Color pixelColor = (maze[j + i * MAZE_SIZE].isWall ? sf::Color::Black : sf::Color::White);
                mazeImage.setPixel(j, i, pixelColor);
            }
            
            // Store image in texture (transition to sprite)            
            tex.loadFromImage(mazeImage);
            
            if(isFinished)
                mazeImage.saveToFile("output.png");
        }
        
        // Create sprite from texture
        sf::Sprite mazeSprite(tex);
        mazeSprite.scale(scaleRatio, scaleRatio);
        
        window.draw(mazeSprite);
        window.display();
    }
    
    return 0;
}
