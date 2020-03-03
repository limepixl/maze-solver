#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

struct Cell
{
    int x, y;
    bool isWall;
    bool visited;
    bool solution;

    Cell() = default;
    Cell(int x, int y) : x(x), y(y)
    {
        isWall = true;
        visited = false;
        solution = false;
    }
};

struct Maze
{
	int size;
    int startX, endX;
	std::vector<Cell> maze;
	
	Maze(int size);
    
    void GenerateMazeData();
};

// Generation algorithms
void DepthFirstGen(Maze& maze, int size, sf::Image& image, sf::RenderWindow& window);
Maze RandomizedPrims(int size, sf::Image& image, sf::RenderWindow& window);

// Solving algorithms
void DepthFirstSearch(Maze& maze, sf::Image& image, sf::RenderWindow& window);
