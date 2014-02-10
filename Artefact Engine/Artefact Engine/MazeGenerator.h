#pragma once

#include <vector>
#include <stack>

class MazeGenerator
{
public:
	static const char MAZE_HALLWAY = '.';
	static const char MAZE_WALL = '#';

	MazeGenerator(const int size);
	~MazeGenerator();
	
	char* generate();
private:
	struct Direction
	{
		int xDir;
		int yDir;
		void turnNorth() { xDir = -1; yDir = 0; }
		void turnSouth() { xDir = 1; yDir = 0; }
		void turnWest() { xDir = 0; yDir = -1; }
		void turnEast() { xDir = 0; yDir = 1; }
	};

	float chanceForChange = 0.2f; //if you give every direction a certain chance you could generate spirals with something like straight: 0.6, left: 0.3, right: 0.1

	struct Position
	{
		int xPos;
		int yPos;

		bool unvisitedNorth = true;
		bool unvisitedEast = true;
		bool unvisitedSouth = true;
		bool unvisitedWest = true;
	};

	struct Digger
	{
		Position position;
		Direction facing;
		std::stack<Position> backTrackStack;
		void dig(char* maze, const int mazeSize, Position newPosition);
	};

	Digger digger;
	char* maze;
	const int mazeSize;
	Position startPos;

	void initializeMaze();
	void initializeDigger();

	void advance();
};

