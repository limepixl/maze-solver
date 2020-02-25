#pragma once
#include <SFML/Graphics.hpp>

struct Maze
{
	int size;
    int startX, endX;
	struct Cell* maze;
    
    int stackIndex;
    struct Cell** pathStack;
	
	Maze(int size);
	~Maze();
    
    void GenerateMazeData();
    bool Iterate(sf::Image& image);
};

