#pragma once

#include <vector>
#include <stack>
#include <iostream>

class MazeGenerator
{
public:
	
	MazeGenerator(const int size);
	~MazeGenerator();
	
	char* generate();
private:
	struct Position
	{
		int xPos;
		int yPos;

		bool unvisitedNorth = true;
		bool unvisitedEast = true;
		bool unvisitedSouth = true;
		bool unvisitedWest = true;
	};


	struct Direction
	{
		const std::string NORTH = "NORTH";
		const std::string SOUTH = "SOUTH";
		const std::string WEST = "WEST";
		const std::string EAST = "EAST";

		std::string dir;
		int xDir;
		int yDir;

		void turnNorth() { xDir = 0; yDir = -2; dir = NORTH; }
		void turnSouth() { xDir = 0; yDir = 2; dir = SOUTH; }
		void turnWest() { xDir = -2; yDir = 0; dir = WEST; }
		void turnEast() { xDir = 2; yDir = 0; dir = EAST; }
		
		void turnRight();
		void turnLeft();
	};

	typedef struct Digger
	{
		Position position;
		Direction facing;
		std::stack<Position> backTrackStack;
		bool dig(char* maze, const int mazeSize);
		void choseDirection();
	} Digger;
	float chanceForChange = 0.2f; //if you	give every direction a certain chance you could generate spirals with something like straight: 0.6, left: 0.3, right: 0.1



	Digger digger;


	char* maze;
	const int mazeSize;
	Position startPos;

	void initializeMaze();
	void initializeDigger();

	void advance();
};

