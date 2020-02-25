#include "maze.h"
#include <ctime>
#include <cstdlib>

struct Cell
{
    int x, y;
    bool isWall;
    bool visited;
    
    Cell() = default;
    Cell(int x, int y) : x(x), y(y)
    {
        isWall = true;
        visited = false;
    }
};

enum Direction
{
    LEFT,
    RIGHT,
    UP,
    DOWN
};

struct Neighbor
{
    Cell* cell;
    Direction dir;
};

Maze::Maze(int size) : size(size)
{
	maze = new Cell[size*size];
	pathStack = nullptr;
	
	GenerateMazeData();
}

Maze::~Maze()
{
	delete[] maze;
}

void Maze::GenerateMazeData()
{
	srand(time(nullptr));   // Seed random number generation
    
    startX = rand() % (size - 2) + 1;
    endX = rand() % (size - 2) + 1;

    // Make start and end even numbers
    startX += (startX % 2 == 0);
    endX += (endX % 2 == 0);
    for(int i = 0; i < size; i++)
    for(int j = 0; j < size; j++)
    {            
        maze[j + i*size] = Cell(j, i);
        
        if(i == 0 && j == startX)
            maze[j + i*size].isWall = false;
        else if(i == size-1 && j == endX)
            maze[j + i*size].isWall = false;
    }
}

bool Maze::Iterate(sf::Image& image)
{
	if(!pathStack)
	{
		pathStack = new Cell*[size*size];
		pathStack[stackIndex = 0] = &maze[startX + size];
	}
	
	Cell* current = pathStack[stackIndex];
	
    int nCount = 0;
    Neighbor neighbors[4];
    neighbors[0].cell = neighbors[1].cell = neighbors[2].cell = neighbors[3].cell = nullptr;
    
    if(current->x - 2 >= 1 && maze[current->x - 2 + current->y*size].isWall) // left
    {
        neighbors[nCount].cell = &maze[current->x - 2 + current->y*size];
        neighbors[nCount++].dir = LEFT;
    }
    if(current->x + 2 <= size - 2 && maze[current->x + 2 + current->y*size].isWall) // right
    {
        neighbors[nCount].cell = &maze[current->x + 2 + current->y*size];
        neighbors[nCount++].dir = RIGHT;
    }
    if(current->y - 2 >= 1 && maze[current->x + (current->y - 2)*size].isWall) // up
    {
        neighbors[nCount].cell = &maze[current->x + (current->y - 2)*size];
        neighbors[nCount++].dir = UP;
    }
    if(current->y + 2 <= size - 2 && maze[current->x + (current->y + 2)*size].isWall) // down
    {
        neighbors[nCount].cell = &maze[current->x + (current->y + 2)*size];
        neighbors[nCount++].dir = DOWN;
    }
    
    // No neighbors
    if(nCount == 0)
    {
        current = pathStack[--stackIndex];
        
        if(stackIndex == 0)
        {
			delete[] pathStack;
			return true;
		}
        
        return false;
    }
    
    int r = rand() % nCount;
    neighbors[r].cell->isWall = false;
    if(neighbors[r].dir == LEFT)
        maze[current->x - 1 + current->y*size].isWall = false;
    else if(neighbors[r].dir == RIGHT)
        maze[current->x + 1 + current->y*size].isWall = false;
    else if(neighbors[r].dir == UP)
        maze[current->x + (current->y - 1)*size].isWall = false;
    else
        maze[current->x + (current->y + 1)*size].isWall = false;
    
    pathStack[++stackIndex] = neighbors[r].cell;
    current = pathStack[stackIndex];
	
	// Create image from maze data
	for(int i = 0; i < size; i++)
	for(int j = 0; j < size; j++)
	{
		sf::Color pixelColor = (maze[j + i * size].isWall ? sf::Color::Black : sf::Color::White);
		image.setPixel(j, i, pixelColor);
	}
    
    return false;
}
