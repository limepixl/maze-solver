#pragma once
#include <ctime>
#include <cstdlib>
#include <cstdio>

// TODO: Move functions into more coherent files

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

void GenerateMaze(Cell maze[], int size, int& start, int& end)
{
    srand(time(nullptr));   // Seed random number generation
    
    start = rand() % (size - 2) + 1;
    end = rand() % (size - 2) + 1;

    // Make start and end even numbers
    start += (start % 2 == 0);
    end += (end % 2 == 0);
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
}

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

bool Iterate(Cell& startCell, Cell* maze, Cell* pathStack, int& arrIndex, int size)
{
    int nCount = 0;
    Neighbor neighbors[4];
    neighbors[0].cell = neighbors[1].cell = neighbors[2].cell = neighbors[3].cell = nullptr;
    
    if(startCell.x - 2 >= 1 && maze[startCell.x - 2 + startCell.y*size].isWall) // left
    {
        neighbors[nCount].cell = &maze[startCell.x - 2 + startCell.y*size];
        neighbors[nCount++].dir = LEFT;
    }
    if(startCell.x + 2 <= size - 2 && maze[startCell.x + 2 + startCell.y*size].isWall) // right
    {
        neighbors[nCount].cell = &maze[startCell.x + 2 + startCell.y*size];
        neighbors[nCount++].dir = RIGHT;
    }
    if(startCell.y - 2 >= 1 && maze[startCell.x + (startCell.y - 2)*size].isWall) // up
    {
        neighbors[nCount].cell = &maze[startCell.x + (startCell.y - 2)*size];
        neighbors[nCount++].dir = UP;
    }
    if(startCell.y + 2 <= size - 2 && maze[startCell.x + (startCell.y + 2)*size].isWall) // down
    {
        neighbors[nCount].cell = &maze[startCell.x + (startCell.y + 2)*size];
        neighbors[nCount++].dir = DOWN;
    }
    
    // No neighbors
    if(nCount == 0)
    {
        startCell = pathStack[--arrIndex];
        
        if(arrIndex == 0)
            return true;
        
        return false;
    }
    
    int r = rand() % nCount;
    neighbors[r].cell->isWall = false;
    if(neighbors[r].dir == LEFT)
        maze[startCell.x - 1 + startCell.y*size].isWall = false;
    else if(neighbors[r].dir == RIGHT)
        maze[startCell.x + 1 + startCell.y*size].isWall = false;
    else if(neighbors[r].dir == UP)
        maze[startCell.x + (startCell.y - 1)*size].isWall = false;
    else
        maze[startCell.x + (startCell.y + 1)*size].isWall = false;
    
    pathStack[++arrIndex] = *neighbors[r].cell;
    startCell = pathStack[arrIndex];
    
    return false;
}

Cell** DepthFirstSearch(Cell* maze, int size, Cell* start, Cell* end)
{
    int currentIndex = start->x;
    Cell* current = start;
    
    int sIndex = 0;
    Cell** solution = new Cell*[size*size];
    solution[0] = start;
    
    while(current != end)
    {
        current->visited = true;
        
        int nCount = 0;
        Neighbor neighbors[4];
        neighbors[0].cell = neighbors[1].cell = neighbors[2].cell = neighbors[3].cell = nullptr;
        
        if(!maze[currentIndex - 1].isWall && !maze[currentIndex - 1].visited)  // left
        {
            neighbors[nCount].cell = &maze[currentIndex - 1];
            neighbors[nCount++].dir = LEFT;
        }
        if(!maze[currentIndex + 1].isWall && !maze[currentIndex + 1].visited)  // right
        {
            neighbors[nCount].cell = &maze[currentIndex + 1];
            neighbors[nCount++].dir = RIGHT;
        }
        if(current != start && !maze[currentIndex - size].isWall && !maze[currentIndex - size].visited) // up
        {
            neighbors[nCount].cell = &maze[currentIndex - size];
            neighbors[nCount++].dir = UP;
        }
        if(!maze[currentIndex + size].isWall && !maze[currentIndex + size].visited) // down
        {
            neighbors[nCount].cell = &maze[currentIndex + size];
            neighbors[nCount++].dir = DOWN;
        }
        
        if(nCount == 0)
        {
            current = solution[--sIndex];
            currentIndex = current->x + current->y * size;
            continue;
        }
        
        int r = rand() % nCount;
        current = neighbors[r].cell;
        currentIndex = current->x + current->y*size;
        solution[++sIndex] = current;
    }
    
    solution[++sIndex] = nullptr;
    return solution;
}


