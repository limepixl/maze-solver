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

struct BacktrackingNeighbor
{
    Cell* cell;
    Cell* lastCell;
};

Maze::Maze()
{
    size = 0;
}

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
        Cell tmp(j, i);
        
        if(i == 0 && j == startX)
            tmp.isWall = false;
        else if(i == size-1 && j == endX)
            tmp.isWall = false;
     
        maze.push_back(tmp);
    }
}

Maze DepthFirstGen(int size, sf::Image& image, sf::RenderWindow& window)
{
    Maze maze(size);
    int startX = maze.startX;
    std::vector<Cell>& cellmaze = maze.maze;

    std::vector<Cell*> pathStack;
    pathStack.reserve(size*size);
    pathStack.push_back(&cellmaze[startX + size]);

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

    window.setFramerateLimit(0);
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

        Cell* current = pathStack.back();

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
            pathStack.pop_back();

            if(pathStack.size() != 0)
                current = pathStack.back();
            else
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

        pathStack.push_back(neighbors[r].cell);
        current = pathStack.back();

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

    window.setFramerateLimit(30);
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

    std::vector<Neighbor> wallList;
    wallList.reserve(size*size);

    cellmaze[startX].isWall = false;

    // Start is below maze start
    Cell* start = &cellmaze[startX + size];
    start->isWall = false;

    // Add start walls to wall list
    if(start->x > 2) wallList.push_back({&cellmaze[start->x - 1 + size], LEFT});
    if(start->x < size - 2) wallList.push_back({&cellmaze[start->x + 1 + size], RIGHT});
    wallList.push_back({&cellmaze[start->x + 2*size], DOWN});

    // Initial draw
    for(int i = 0; i < size; i++)
    for(int j = 0; j < size; j++)
    {
        sf::Color pixelColor = (cellmaze[j + i * size].isWall ? sf::Color::Black : sf::Color::White);
        image.setPixel(j, i, pixelColor);
    }

    window.setFramerateLimit(0);
    while(wallList.size() != 0)
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

        int randomWallIndex = rand() % wallList.size();
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
            auto contains = [](std::vector<Neighbor>& neighbors, Cell* cell)
            {
                bool res = false;
                for(auto& n : neighbors)
                if(n.cell == cell)
                {
                    res = true;
                    break;
                }

                return res;
            };

            // Add neighboring walls of newly added cell to the wall list
            if(cellToMark->x < size - 2 && cellmaze[cellToMark->x + 1 + cellToMark->y*size].isWall && !contains(wallList, &cellmaze[cellToMark->x + 1 + cellToMark->y*size]))
                wallList.push_back({&cellmaze[cellToMark->x + 1 + cellToMark->y*size], RIGHT});
            if(cellToMark->x > 2 && cellmaze[cellToMark->x - 1 + cellToMark->y*size].isWall && !contains(wallList, &cellmaze[cellToMark->x - 1 + cellToMark->y*size]))
                wallList.push_back({&cellmaze[cellToMark->x - 1 + cellToMark->y*size], LEFT});
            if(cellToMark->y < size - 2 && cellmaze[cellToMark->x + cellToMark->y*size + size].isWall && !contains(wallList, &cellmaze[cellToMark->x + size + cellToMark->y*size]))
                wallList.push_back({&cellmaze[cellToMark->x + cellToMark->y*size + size], DOWN});
            if(cellToMark->y > 2 && cellmaze[cellToMark->x + cellToMark->y*size - size].isWall && !contains(wallList, &cellmaze[cellToMark->x - size + cellToMark->y*size]))
                wallList.push_back({&cellmaze[cellToMark->x + cellToMark->y*size - size], UP});
        }

        // Overwrite current wall in list with last wall
        wallList.erase(wallList.begin() + randomWallIndex);

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

    window.setFramerateLimit(30);
    return maze;
}
    
void DepthFirstSearch(Maze& m, sf::Image& image, sf::RenderWindow& window)
{
    // If there isn't a generated maze yet, do nothing
    if(!m.size)
    {
        printf("You need to generate a maze first.\n");
        return;
    }

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

    std::vector<Cell*> pathStack;
    pathStack.reserve(size*size);
    pathStack.push_back(&m.maze[startX]);

    Cell* current = pathStack.back();
    Cell* start = &m.maze[startX];
    Cell* end = &m.maze[endX + size*size - size];

    sf::Texture tex;
    sf::Sprite mazeSprite;
    float scaleRatio = (float)window.getSize().x / size;

    window.setFramerateLimit(0);
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
            if(pathStack.size() == 0)
                break;

            image.setPixel(current->x, current->y, sf::Color::White);
            current->solution = false;
            pathStack.pop_back();
            current = pathStack.back();
            continue;
        }

        int r = rand() % nCount;
        current = neighbors[r].cell;
        pathStack.push_back(current);

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

    window.setFramerateLimit(30);
}

// TODO: Improve because it isn't that efficient
void BreadthFirstSearch(Maze &m, sf::Image &image, sf::RenderWindow &window)
{
    // If there isn't a generated maze yet, do nothing
    if(!m.size)
    {
        printf("You need to generate a maze first.\n");
        return;
    }

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

    Cell* start = &m.maze[startX];
    Cell* end = &m.maze[endX + size*size - size];
    Cell* current = start;
    image.setPixel(current->x, current->y, sf::Color::Blue);

    int queueIndex = 1;
    std::vector<BacktrackingNeighbor> queue;
    queue.reserve(size*size);
    queue.push_back({start, nullptr});

    sf::Texture tex;
    sf::Sprite mazeSprite;
    float scaleRatio = (float)window.getSize().x / size;

    window.setFramerateLimit(0);
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

        if(!m.maze[current->x - 1 + current->y*size].isWall && !m.maze[current->x - 1 + current->y*size].visited)
            queue.push_back({&m.maze[current->x - 1 + current->y * size], current});
        if(!m.maze[current->x + current->y*size + size].isWall && !m.maze[current->x + current->y*size + size].visited)
            queue.push_back({&m.maze[current->x + current->y*size + size], current});
        if(!m.maze[current->x + 1 + current->y*size].isWall && !m.maze[current->x + 1 + current->y*size].visited)
            queue.push_back({&m.maze[current->x + 1 + current->y*size], current});
        if(current != start && !m.maze[current->x + current->y*size - size].isWall && !m.maze[current->x + current->y*size - size].visited)
            queue.push_back({&m.maze[current->x + current->y*size - size], current});

        current->visited = true;
        current = queue[queueIndex++].cell;
        image.setPixel(current->x, current->y, sf::Color::Blue);

        if(current == end)
        {
            current->solution = true;
            for(BacktrackingNeighbor& last = queue[queueIndex-1]; last.cell != start;)
            {
                Cell* lastCell = last.lastCell;

                for(size_t j = queue.size()-1; ; j--)
                if(queue[j].cell == lastCell)
                {
                    lastCell->solution = true;
                    last = queue[j];
                    break;
                }
            }

            for(int i = 0; i < size; i++)
            for(int j = 0; j < size; j++)
            if(image.getPixel(j, i) == sf::Color::Blue)
            {
                if(m.maze[j+i*size].solution)
                    image.setPixel(j, i, sf::Color::Red);
                else
                    image.setPixel(j, i, sf::Color::White);
            }

            printf("Finished solving maze!\n");
            image.saveToFile("solved.png");

            break;
        }

        // Store image in texture (transition to sprite)
        tex.loadFromImage(image);

        // Create sprite from texture
        mazeSprite = sf::Sprite(tex);
        mazeSprite.scale(scaleRatio, scaleRatio);

        window.draw(mazeSprite);
        window.display();
    }

    window.setFramerateLimit(30);
}
