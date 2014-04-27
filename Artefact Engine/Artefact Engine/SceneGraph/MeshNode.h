#pragma once

#include "SceneNode.h"
#include <glm\glm.hpp>
#include <FreeImage\FreeImage.h>
#include <vector>
#include <PhysX\PxPhysicsAPI.h>
//#include <NxPhysics


class MeshNode : public SceneNode
{
public:
	MeshNode(glm::vec3 startPos);
	~MeshNode();

	void update(double dT, InputHandler* input);
	void draw(glm::mat4 vp);

	void initializeMeshNode(std::string identifyer, physx::PxScene* physicsScene, physx::PxPhysics* physicsSDK);
	void bind();
	
	//void DrawShape(NxShape* shape)

	//void setTransform(physx::PxTransform transform);


private:
	std::string path;

	GLuint vao;

	GLuint vbo; //vertex
	std::vector<glm::vec3> vertices;
	GLuint ibo; //index
	std::vector<unsigned int> faces;
	GLuint nbo; //normals
	std::vector<float> normals;
	GLuint uvbo; //UV coordinates
	std::vector<glm::vec2> uvs;
	GLuint wbo; //weights
	std::vector<float> weights;
	GLuint cbo; //color
	FIBITMAP* texture;
	GLuint texbo;
	
	GLuint shaderProgram;
	
	

	GLuint mvpLocation;		//mvp identifier for shader
	glm::mat4 mvp;

	glm::mat4 modelMatrix;
};

