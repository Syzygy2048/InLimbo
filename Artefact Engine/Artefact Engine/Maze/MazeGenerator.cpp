#include "MazeGenerator.h"

#include <iostream>
/* profiling
#include <time.h> */

#include <ctime>

#include "../Enums/MazeEnums.h"
MazeGenerator::MazeGenerator(const int size) : mazeSize(size)
{
}


MazeGenerator::~MazeGenerator()
{
}

//Generates a 2D int matrix of size mazeSize and fills it with an imperfect maze (DepthFirst-Digger-Algorithm with removal of semi randomly selected areas to make it imperfect)
char* MazeGenerator::generate()
{
	/* profiling
	clock_t begin = clock(); */
	srand(time(NULL));
	initializeMaze();
	initializeDigger();
	do{
		if(digger.position.unvisitedEast || digger.position.unvisitedNorth || digger.position.unvisitedSouth || digger.position.unvisitedWest)
		{
			do{
				digger.choseDirection();
			} while (!digger.dig(maze, mazeSize) && (digger.position.unvisitedEast || digger.position.unvisitedNorth || digger.position.unvisitedSouth || digger.position.unvisitedWest));
		}
		if (!(digger.position.unvisitedEast || digger.position.unvisitedNorth || digger.position.unvisitedSouth || digger.position.unvisitedWest))
		{
			digger.position = digger.backTrackStack.top(); //would be peek in other languages
			digger.backTrackStack.pop();				//would return the popped element in other languages but does not.
		}
	} while (!(startPos.xPos == digger.position.xPos && startPos.yPos == digger.position.yPos));

	/* profiling
	clock_t end = clock();
	std::cerr << double(end-begin)/CLOCKS_PER_SEC << std::endl;  */
	
	//remove random blocks to make the maze imperfect
	for (int i = 0; i <= mazeSize; i++) //removes mazeSize semi random blocks from the maze
	{
		int posX = (rand() % (mazeSize / 2 - 2)) * 2 + 2;	//we only want to check for walls here, this is guaranteed to be an even number, which makes it more likely to be a wall
		int posY = (rand() % (mazeSize / 2 - 2)) * 2 + 2;	// -2 and +2 to remove cases where the following checks would go out of bounds

		if (maze[posX + posY * mazeSize] == MAZE_WALL &&
			(maze[posX + 1 + posY * mazeSize] == MAZE_WALL || maze[posX - 1 + posY * mazeSize] == MAZE_WALL) !=
			(maze[posX + (posY + 1)* mazeSize] == MAZE_WALL || maze[posX + (posY - 1) * mazeSize] == MAZE_WALL)
			&&
			(maze[posX + 1 + posY * mazeSize] == MAZE_WALL && maze[posX - 1 + posY * mazeSize] == MAZE_WALL) !=
			(maze[posX + (posY + 1)* mazeSize] == MAZE_WALL && maze[posX + (posY - 1) * mazeSize] == MAZE_WALL))
		{ 
				maze[posX + posY * mazeSize] = REMOVED_BLOCK;
		}
		else
		{
			i--;
		}
	}

	for (int i = 0; i < mazeSize; i++)
	{
		for (int j = 0; j < mazeSize; j++)
		{
			std::cerr << maze[j + i * mazeSize];
		}
		std::cerr << std::endl;
	}

	return maze;
}

//prepare the maze for use of a "digger" maze algorithm (create all walls and then destroy those in it's way)
void MazeGenerator::initializeMaze()
{
	maze = new char[mazeSize * mazeSize];

	for (int i = 0; i < mazeSize; i++)
	{
		for (int j = 0; j < mazeSize; j++)
		{
			if (i % 2 == 0 || j % 2 == 0)
			{
				maze[j + i * mazeSize] = MAZE_WALL;
			}
			else
			{
				maze[j + i * mazeSize] = MAZE_HALLWAY;
			}
		}
	}
}

//prepares the digger at a random position and looking into a random direction. 
//might be better to start at the top x: mazeSize/2+1, y: 1 and look south and have the tower south
void MazeGenerator::initializeDigger()
{
	int xPos = (rand() % (mazeSize / 2)) * 2 + 1;
	int yPos = (rand() % (mazeSize / 2)) * 2 + 1;
	digger.position.xPos = xPos;
	digger.position.yPos = yPos;
	startPos.xPos = xPos;
	startPos.yPos = yPos;
	switch (rand() % 4)
	{
		case 0: digger.facing.turnNorth(); break;
		case 1: digger.facing.turnEast(); break;
		case 2: digger.facing.turnSouth(); break;
		case 3: digger.facing.turnWest(); break;
	}
	digger.backTrackStack = std::stack<Position>();

}


//tries to dig into the 
bool MazeGenerator::Digger::dig(char* maze, const int mazeSize)
{	
	MazeGenerator::Position newPosition;
	newPosition.xPos = position.xPos + facing.xDir;
	newPosition.yPos = position.yPos + facing.yDir;

	if ((newPosition.xPos < 1 || newPosition.xPos > mazeSize - 1 || newPosition.yPos < 1 || newPosition.yPos > mazeSize - 1) //check if new position is out of bounds
		|| (maze[newPosition.xPos + 1 + newPosition.yPos * mazeSize] == MAZE_HALLWAY
		|| maze[newPosition.xPos - 1 + newPosition.yPos * mazeSize] == MAZE_HALLWAY
		|| maze[newPosition.xPos + (newPosition.yPos + 1) * mazeSize] == MAZE_HALLWAY
		|| maze[newPosition.xPos + (newPosition.yPos - 1) * mazeSize] == MAZE_HALLWAY)) //or already visited
		 
	{
		if (facing.xDir > 0) position.unvisitedWest = false;
		else if (facing.xDir < 0) position.unvisitedEast = false;
		else if (facing.yDir > 0) position.unvisitedSouth = false;
		else if (facing.yDir < 0) position.unvisitedNorth = false;
		return false;
	}
	else    //put old position in stack for backtracking, removes the wall between the old and new position and sets the new position as the current position
	{
		if (position.xPos - newPosition.xPos < 0)
		{ 
			newPosition.unvisitedWest = false;
			maze[newPosition.xPos - 1 + newPosition.yPos * mazeSize] = MAZE_HALLWAY;
		}
		else if (position.xPos - newPosition.xPos > 0)
		{
			newPosition.unvisitedEast = false;
			maze[newPosition.xPos + 1 + newPosition.yPos * mazeSize] = MAZE_HALLWAY;
		}
		else if (position.yPos - newPosition.yPos < 0)
		{
			newPosition.unvisitedNorth = false;
			maze[newPosition.xPos + (newPosition.yPos - 1)  * mazeSize] = MAZE_HALLWAY;
		}
		else if (position.yPos - newPosition.yPos > 0)
		{
			newPosition.unvisitedSouth = false;
			maze[newPosition.xPos + (newPosition.yPos + 1)  * mazeSize] = MAZE_HALLWAY;
		}
		backTrackStack.push(position);
		position = newPosition;
		return true;
	}
}

void MazeGenerator::Digger::choseDirection()
{
	int newDir = rand() % 100;
	if (newDir < 20)
	{
		facing.turnLeft();
	}
	else if (newDir > 80)
	{	
		facing.turnRight();
	}

}

void MazeGenerator::Direction::turnRight()
{
	if (dir == NORTH) turnEast();
	else if (dir == SOUTH) turnWest();
	else if (dir == EAST) turnSouth();
	else if (dir == WEST) turnNorth();
}

void MazeGenerator::Direction::turnLeft()
{
	if (dir == NORTH) turnWest();
	else if (dir == SOUTH) turnEast();
	else if (dir == EAST) turnNorth();
	else if (dir == WEST) turnSouth();
}