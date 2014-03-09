#pragma once

#include <iostream>
#include <vector>

class MazeTile
{
public:
	//this needs to be an uneven number
	static const int TILE_SIZE = 79; 
	
	static const int NEIGHBOR_TOP = 0;
	static const int NEIGHBOR_TOP_RIGHT = 1;
	static const int NEIGHBOR_RIGHT = 2;
	static const int NEIGHBOR_BOTTOM_RIGHT = 3;
	static const int NEIGHBOR_BOTTOM = 4;
	static const int NEIGHBOR_BOTTOM_LEFT = 5;
	static const int NEIGHBOR_LEFT = 6;
	static const int NEIGHBOR_TOP_LEFT = 7;
	
	MazeTile();
	~MazeTile();
	
	//sets the neighboring maze tile to this tile,use the constants defined in MazeTile for the relativePosition
	void setNeighbor(MazeTile* neighbor, int relativePosition);

private:
	char* tile;
	MazeTile* top;
	MazeTile* topRight;
	MazeTile* right;
	MazeTile* bottomRight;
	MazeTile* bottom;
	MazeTile* bottomLeft;
	MazeTile* left;
	MazeTile* topLeft;
};

