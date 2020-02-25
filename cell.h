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

void GenerateMaze(Cell* maze, int size, int& start, int& end)
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
    
    // TODO: Find better way to pick direction
    int r = rand() % 4;
    while(neighbors[r] == nullptr /*|| !neighbors[r]->isWall*/)
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
        Cell* neighbors[4];
        neighbors[0] = neighbors[1] = neighbors[2] = neighbors[3] = nullptr;
        
        if(!maze[currentIndex - 1].isWall && !maze[currentIndex - 1].visited)  // left
        {
            neighbors[0] = &maze[currentIndex - 1];
            nCount++;
        }
        if(!maze[currentIndex + 1].isWall && !maze[currentIndex + 1].visited)  // right
        {
            neighbors[1] = &maze[currentIndex + 1];
            nCount++;
        }
        if(current != start && !maze[currentIndex - size].isWall && !maze[currentIndex - size].visited) // up
        {
            neighbors[2] = &maze[currentIndex - size];
            nCount++;
        }
        if(!maze[currentIndex + size].isWall && !maze[currentIndex + size].visited) // down
        {
            neighbors[3] = &maze[currentIndex + size];
            nCount++;
        }
        
        if(nCount == 0)
        {
            current = solution[--sIndex];
            currentIndex = current->x + current->y * size;
            continue;
        }
        
        int dir = rand() % 4;
        while(neighbors[dir] == nullptr)
            dir = rand() % 4;
 
        current = neighbors[dir];
        currentIndex = current->x + current->y*size;
        solution[++sIndex] = current;
    }
    
    solution[++sIndex] = nullptr;
    return solution;
}


