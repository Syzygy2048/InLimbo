#include "MazeGenerator.h"

#include <iostream>
#include <time.h>

MazeGenerator::MazeGenerator(const int size) :  mazeSize(size)
{
}


MazeGenerator::~MazeGenerator()
{
}

//Generates a 2D int matrix of size mazeSize and fills it with an imperfect maze (DepthFirst-Digger-Algorithm with removal of semi randomly selected areas to make it imperfect)
char* MazeGenerator::generate()
{
	srand(time(NULL));
	initializeMaze();
	initializeDigger();
	
	do{
		choseDirection();
		advanceDigger();
	}while (startPos.xPos != digger.position.xPos && startPos.yPos != digger.position.yPos)

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
void MazeGenerator::Digger::dig(char* maze, const int mazeSize, Position newPosition)
{	
	if (!(newPosition.xPos > 0 && newPosition.xPos < mazeSize && newPosition.yPos > 0 && newPosition.xPos < mazeSize) //check if new position is out of bounds
		&& maze[newPosition.xPos + 1 + newPosition.yPos * mazeSize] == MAZE_HALLWAY
		|| maze[newPosition.xPos - 1 + newPosition.yPos * mazeSize] == MAZE_HALLWAY
		|| maze[newPosition.xPos + newPosition.yPos + 1 * mazeSize] == MAZE_HALLWAY
		|| maze[newPosition.xPos + newPosition.yPos - 1 * mazeSize] == MAZE_HALLWAY) //or already visited
		 
	{
		if (position.xPos - newPosition.xPos == -1) position.unvisitedWest = false;
		else if (position.xPos - newPosition.xPos == 1) position.unvisitedEast = false;
		else if (position.yPos - newPosition.yPos == -1) position.unvisitedNorth = false;
		else if (position.yPos - newPosition.yPos == 1) position.unvisitedSouth = false;
	}
	else    //put old position in stack for backtracking, removes the wall between the old and new position and sets the new position as the current position
	{
		if (position.xPos - newPosition.xPos < 0)
		{ 
			newPosition.unvisitedEast = false;
			maze[newPosition.xPos - 1 + newPosition.yPos * mazeSize] = MAZE_HALLWAY;
		}
		else if (position.xPos - newPosition.xPos > 0)
		{
			newPosition.unvisitedWest = false;
			maze[newPosition.xPos + 1 + newPosition.yPos * mazeSize] = MAZE_HALLWAY;
		}
		else if (position.yPos - newPosition.yPos < 0)
		{
			newPosition.unvisitedSouth = false;
			maze[newPosition.xPos + newPosition.yPos - 1  * mazeSize] = MAZE_HALLWAY;
		}
		else if (position.yPos - newPosition.yPos > 0)
		{
			newPosition.unvisitedNorth = false;
			maze[newPosition.xPos + newPosition.yPos + 1  * mazeSize] = MAZE_HALLWAY;
		}
		backTrackStack.push(position);
		position = newPosition;
	}
}