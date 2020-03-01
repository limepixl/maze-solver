#pragma once
#include <SFML/Graphics.hpp>

struct Maze
{
	unsigned size;
    unsigned startX, endX;
	struct Cell* maze;
	
	Maze(unsigned size);
	~Maze();
    
    void GenerateMazeData();

    // Generation algorithms
    void DepthFirstGen(sf::Image& image, sf::RenderWindow& window);
    void RandomizedPrims(sf::Image& image, sf::RenderWindow& window);

    // Solving algorithms
    void DepthFirstSearch(sf::Image& image, sf::RenderWindow& window);
};

