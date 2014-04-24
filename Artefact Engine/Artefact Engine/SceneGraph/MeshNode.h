#pragma once

#include "SceneNode.h"
#include <glm\glm.hpp>


class MeshNode : public SceneNode
{
public:
	MeshNode(glm::vec3 startPos);
	MeshNode(MeshNode* meshNode, glm::vec3 startPos);

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
	void loadMesh();
	void bind();
	

private:
	const std::string path;
	//std::vector<glm::vec3> vertices;
	//std::vector<unsigned int> faces;
	GLuint vao;

	GLuint vbo; //vertex
	std::vector<float> vertices;
	GLuint ibo; //index
	std::vector<unsigned int> indices;
	GLuint nbo; //normals
	std::vector<float> normals;
	GLuint uvbo; //UV coordinates
	std::vector<unsigned int> uvs;
	GLuint wbo; //weights
	std::vector<float> weights;
	GLuint cbo; //color
	
	GLuint shaderProgram;
	
	

	GLuint mvpLocation;		//mvp identifier for shader
	glm::mat4 mvp;

	glm::mat4 modelMatrix;

	int numberOfFaces;
};

