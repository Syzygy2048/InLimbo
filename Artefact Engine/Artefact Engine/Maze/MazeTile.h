#pragma once

#include <iostream>
#include <vector>
#include "../SceneGraph/SceneNode.h"
#include "../Enums/SceneGraphEnums.h"


class MazeTile : public SceneNode
{
public:
	//this needs to be an uneven number
	static const int TILE_SIZE = 25; 
	
	static const int NEIGHBOR_TOP = 0;
	static const int NEIGHBOR_TOP_RIGHT = 1;
	static const int NEIGHBOR_RIGHT = 2;
	static const int NEIGHBOR_BOTTOM_RIGHT = 3;
	static const int NEIGHBOR_BOTTOM = 4;
	static const int NEIGHBOR_BOTTOM_LEFT = 5;
	static const int NEIGHBOR_LEFT = 6;
	static const int NEIGHBOR_TOP_LEFT = 7;
	
	void bind();

	void update(double dT, InputHandler* input);
	void draw(glm::mat4 vp);

	MazeTile();
	~MazeTile();
	
	//sets the neighboring maze tile to this tile,use the constants defined in MazeTile for the relativePosition
	void setNeighbor(MazeTile* neighbor, int relativePosition);

	char* tile;

	void mergeAsMesh();
private:
	
	MazeTile* top;
	MazeTile* topRight;
	MazeTile* right;
	MazeTile* bottomRight;
	MazeTile* bottom;
	MazeTile* bottomLeft;
	MazeTile* left;
	MazeTile* topLeft;

	GLuint vao;

	GLuint vbo;
	std::vector<glm::vec3> vertices;
	GLuint ibo;
	std::vector<unsigned int> faces;

	GLuint shaderProgram;
	GLuint mvpLocation;
	

	glm::mat4 mvp;
	glm::mat4 modelMatrix;
};

