#pragma once

#include "SceneNode.h"
#include <glm\glm.hpp>


class MeshNode : public SceneNode
{
public:
	MeshNode(std::string path);
	~MeshNode();

	void update(double dT, InputHandler* input);
	void draw(glm::mat4 vp);
	
	void setVao(GLuint v){ vao = v; }
	void setUvBuffer(GLuint vbo){ uvbo = vbo; }
	void setVertexBuffer(GLuint vbo){ this->vbo = vbo; }
	void setNormBuffer(GLuint vbo){ nbo = vbo; }
	void setWeightBuffer(GLuint vbo){ wbo = vbo; }
	void setIndexBuffer(GLuint vbo){ ibo = vbo; }
	void setNumberOfFaces(int numOfFaces) { numberOfFaces = numOfFaces; }



private:
	const std::string path;

	GLuint vao;

	GLuint vbo; //vertex
	GLuint ibo; //index
	GLuint nbo; //normals
	GLuint uvbo; //UV coordinates
	GLuint wbo; //weights
	GLuint cbo; //color
	
	GLuint shaderProgram;


	GLuint mvpLocation;		//mvp identifier for shader
	glm::mat4 mvp;

	glm::mat4 modelMatrix;

	int numberOfFaces;
};

