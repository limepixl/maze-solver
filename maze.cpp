#include "maze.h"

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
	maze.reserve(size*size);
	GenerateMazeData();
}

void Maze::GenerateMazeData()
{
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

Maze DepthFirstGen(int size, sf::Image& image, sf::RenderWindow& window)
{
    Maze maze(size);
    int startX = maze.startX;
    std::vector<Cell>& cellmaze = maze.maze;

    int stackIndex = 0;
    Cell* pathStack[size*size];
    pathStack[0] = &cellmaze[startX + size];

    sf::Texture tex;
    sf::Sprite mazeSprite;
    float scaleRatio = (float)window.getSize().x / size;

    // Initial draw
    for(int i = 0; i < size; i++)
    for(int j = 0; j < size; j++)
    {
        sf::Color pixelColor = (cellmaze[j + i * size].isWall ? sf::Color::Black : sf::Color::White);
        image.setPixel(j, i, pixelColor);
    }

    while(true)
    {
        sf::Event e;
        while(window.pollEvent(e))
        {
            if(e.type == sf::Event::Closed)
            {
                window.close();
                exit(-1);
            }
        }
        window.clear(sf::Color::Blue);

        Cell* current = pathStack[stackIndex];

        int nCount = 0;
        Neighbor neighbors[4];
        neighbors[0].cell = neighbors[1].cell = neighbors[2].cell = neighbors[3].cell = nullptr;

        if(current->x - 2 >= 1 && cellmaze[current->x - 2 + current->y*size].isWall)
            neighbors[nCount++] = {&cellmaze[current->x - 2 + current->y*size], LEFT};

        if(current->x + 2 <= size - 2 && cellmaze[current->x + 2 + current->y*size].isWall)
            neighbors[nCount++] = {&cellmaze[current->x + 2 + current->y*size], RIGHT};

        if(current->y - 2 >= 1 && cellmaze[current->x + (current->y - 2)*size].isWall)
            neighbors[nCount++] = {&cellmaze[current->x + (current->y - 2)*size], UP};

        if(current->y + 2 <= size - 2 && cellmaze[current->x + (current->y + 2)*size].isWall)
            neighbors[nCount++] = {&cellmaze[current->x + (current->y + 2)*size], DOWN};

        // No neighbors
        if(nCount == 0)
        {
            current = pathStack[--stackIndex];

            if(stackIndex == 0)
                break;

            continue;
        }

        int r = rand() % nCount;
        neighbors[r].cell->isWall = false;
        image.setPixel(neighbors[r].cell->x, neighbors[r].cell->y, sf::Color::White);

        Cell* cellToAdd;
        if(neighbors[r].dir == LEFT)
            cellToAdd = &cellmaze[current->x - 1 + current->y*size];
        else if(neighbors[r].dir == RIGHT)
            cellToAdd = &cellmaze[current->x + 1 + current->y*size];
        else if(neighbors[r].dir == UP)
            cellToAdd = &cellmaze[current->x + (current->y - 1)*size];
        else
            cellToAdd = &cellmaze[current->x + (current->y + 1)*size];

        cellToAdd->isWall = false;
        image.setPixel(cellToAdd->x, cellToAdd->y, sf::Color::White);

        pathStack[++stackIndex] = neighbors[r].cell;
        current = pathStack[stackIndex];

        // Store image in texture (transition to sprite)
        tex.loadFromImage(image);

        // Create sprite from texture
        mazeSprite = sf::Sprite(tex);
        mazeSprite.scale(scaleRatio, scaleRatio);

        window.draw(mazeSprite);
        window.display();
    }

    // Save generated maze image
    image.saveToFile("unsolved.png");
    printf("Finished generating maze!\n");

    return maze;
}

Maze RandomizedPrims(int size, sf::Image &image, sf::RenderWindow &window)
{
    Maze maze(size);
    int startX = maze.startX;
    std::vector<Cell>& cellmaze = maze.maze;

    sf::Texture tex;
    sf::Sprite mazeSprite;
    float scaleRatio = (float)window.getSize().x / size;

    int wallIndex = 0;
    Neighbor wallList[size*size];

    cellmaze[startX].isWall = false;

    // Start is below maze start
    Cell* start = &cellmaze[startX + size];
    start->isWall = false;

    // Add start walls to wall list
    if(start->x > 2) wallList[wallIndex++] = {&cellmaze[start->x - 1 + size], LEFT};
    if(start->x < size - 2) wallList[wallIndex++] = {&cellmaze[start->x + 1 + size], RIGHT};
    wallList[wallIndex++] = {&cellmaze[start->x + 2*size], DOWN};

    // Initial draw
    for(int i = 0; i < size; i++)
    for(int j = 0; j < size; j++)
    {
        sf::Color pixelColor = (cellmaze[j + i * size].isWall ? sf::Color::Black : sf::Color::White);
        image.setPixel(j, i, pixelColor);
    }

    while(wallIndex != 0)
    {
        sf::Event e;
        while(window.pollEvent(e))
        {
            if(e.type == sf::Event::Closed)
            {
                window.close();
                exit(-1);
            }
        }
        window.clear(sf::Color::Blue);

        int randomWallIndex = rand() % wallIndex;
        Neighbor* randomWall = &wallList[randomWallIndex];

        // Offset to get the cell in the neighbor direction
        int offset = (randomWall->dir == Direction::RIGHT) -
                     (randomWall->dir == Direction::LEFT)  -
                size*(randomWall->dir == Direction::UP)    +
                size*(randomWall->dir == Direction::DOWN);

        Cell* cellToMark = &cellmaze[randomWall->cell->x + randomWall->cell->y*size + offset];
        Cell* lastPathCell = &cellmaze[randomWall->cell->x + randomWall->cell->y*size - offset];

        // If only one of the cells is marked as a path, mark the next one
        bool shouldMark = (!cellToMark->isWall && lastPathCell->isWall) || (cellToMark->isWall && !lastPathCell->isWall);
        if(shouldMark)
        {
            // Connect path to new cell
            cellToMark->isWall = false;
            randomWall->cell->isWall = false;
            image.setPixel(cellToMark->x, cellToMark->y, sf::Color::White);
            image.setPixel(randomWall->cell->x, randomWall->cell->y, sf::Color::White);

            // Avoiding C++ features while still using lambdas :^)
            auto contains = [](Neighbor* neighbors, int size, Cell* cell)
            {
                bool res = false;
                for(int i = 0; i < size; i++)
                if(neighbors[i].cell == cell)
                {
                    res = true;
                    break;
                }

                return res;
            };

            // Add neighboring walls of newly added cell to the wall list
            if(cellToMark->x < size - 2 && cellmaze[cellToMark->x + 1 + cellToMark->y*size].isWall && !contains(wallList, wallIndex, &cellmaze[cellToMark->x + 1 + cellToMark->y*size]))
                wallList[wallIndex++] = {&cellmaze[cellToMark->x + 1 + cellToMark->y*size], RIGHT};
            if(cellToMark->x > 2 && cellmaze[cellToMark->x - 1 + cellToMark->y*size].isWall && !contains(wallList, wallIndex, &cellmaze[cellToMark->x - 1 + cellToMark->y*size]))
                wallList[wallIndex++] = {&cellmaze[cellToMark->x - 1 + cellToMark->y*size], LEFT};
            if(cellToMark->y < size - 2 && cellmaze[cellToMark->x + cellToMark->y*size + size].isWall && !contains(wallList, wallIndex, &cellmaze[cellToMark->x + size + cellToMark->y*size]))
                wallList[wallIndex++] = {&cellmaze[cellToMark->x + cellToMark->y*size + size], DOWN};
            if(cellToMark->y > 2 && cellmaze[cellToMark->x + cellToMark->y*size - size].isWall && !contains(wallList, wallIndex, &cellmaze[cellToMark->x - size + cellToMark->y*size]))
                wallList[wallIndex++] = {&cellmaze[cellToMark->x + cellToMark->y*size - size], UP};
        }

        // Overwrite current wall in list with last wall
        // (This is only necessary because I am trying to
        // avoid the C++ STL, and using C style arrays.)
        wallList[randomWallIndex] = wallList[--wallIndex];

        // Store image in texture (transition to sprite)
        tex.loadFromImage(image);

        // Create sprite from texture
        mazeSprite = sf::Sprite(tex);
        mazeSprite.scale(scaleRatio, scaleRatio);

        window.draw(mazeSprite);
        window.display();
    }

    // Save generated maze image
    image.saveToFile("unsolved.png");
    printf("Finished generating maze!\n");

    return maze;
}
    
void DepthFirstSearch(Maze& m, sf::Image& image, sf::RenderWindow& window)
{
    int size = m.size;
    int startX = m.startX;
    int endX = m.endX;

    // Reset everything if solving for second time
    for(int i = 0; i < m.size*size; i++)
    {
        m.maze[i].solution = false;
        m.maze[i].visited = false;
    }
    for(int i = 0; i < size; i++)
    for(int j = 0; j < size; j++)
    {
        Cell& currentCell = m.maze[j+i*size];
        image.setPixel(j, i, (currentCell.isWall ? sf::Color::Black : sf::Color::White));
    }

    int stackIndex = 0;
    Cell* pathStack[size*size];
    pathStack[0] = &m.maze[startX];

    Cell* current = pathStack[stackIndex];
    Cell* start = &m.maze[startX];
    Cell* end = &m.maze[endX + size*size - size];

    sf::Texture tex;
    sf::Sprite mazeSprite;
    float scaleRatio = (float)window.getSize().x / size;

    while(current != end)
    {
        sf::Event e;
        while(window.pollEvent(e))
        {
            if(e.type == sf::Event::Closed)
            {
                window.close();
                exit(-1);
            }
        }
        window.clear(sf::Color::Blue);

        current->visited = true;
        current->solution = true;
        image.setPixel(current->x, current->y, sf::Color::Red);

        int nCount = 0;
        Neighbor neighbors[4];
        neighbors[0].cell = neighbors[1].cell = neighbors[2].cell = neighbors[3].cell = nullptr;

        if(!m.maze[current->x - 1 + current->y*size].isWall && !m.maze[current->x - 1 + current->y*size].visited)
            neighbors[nCount++] = {&m.maze[current->x - 1 + current->y*size], LEFT};

        if(!m.maze[current->x + 1 + current->y*size].isWall && !m.maze[current->x + 1 + current->y*size].visited)
            neighbors[nCount++] = {&m.maze[current->x + 1 + current->y*size], RIGHT};

        if(current != start && !m.maze[current->x - size + current->y*size].isWall && !m.maze[current->x - size + current->y*size].visited)
            neighbors[nCount++] = {&m.maze[current->x - size + current->y*size], UP};

        if(!m.maze[current->x + size + current->y*size].isWall && !m.maze[current->x + size + current->y*size].visited)
            neighbors[nCount++] = {&m.maze[current->x + size + current->y*size], DOWN};

        if(nCount == 0)
        {
            if(stackIndex == 0)
                break;

            image.setPixel(current->x, current->y, sf::Color::White);
            current->solution = false;
            current = pathStack[--stackIndex];
            continue;
        }

        int r = rand() % nCount;
        current = neighbors[r].cell;
        pathStack[++stackIndex] = current;

        // Color in the solution path
        image.setPixel(current->x, current->y, sf::Color::Red);

        // Store image in texture (transition to sprite)
        tex.loadFromImage(image);

        // Create sprite from texture
        mazeSprite = sf::Sprite(tex);
        mazeSprite.scale(scaleRatio, scaleRatio);

        window.draw(mazeSprite);
        window.display();

        if(current == end)
            break;
    }

    // Save solved maze image
    image.saveToFile("solved.png");
    printf("Finished solving maze!\n");
}
