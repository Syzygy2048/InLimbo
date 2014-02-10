#include "MazeTile.h"
#include "MazeGenerator.h"


MazeTile::MazeTile()
{
	tile = MazeGenerator(TILE_SIZE).generate();
}


MazeTile::~MazeTile()
{
	delete[] tile;
}
