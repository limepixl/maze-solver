#pragma once
#include <ctime>
#include <cstdlib>
#include <cstdio>


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


