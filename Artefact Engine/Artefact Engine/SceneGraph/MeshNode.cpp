

#include <GL\glew.h>

#include "MeshNode.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "../Enums/MazeEnums.h"
#include "../Asset/AssetLoader.h"
#include "../Enums/MeshPaths.h"



#include "../Shaders/ShaderLoader.h"
std::vector<glm::vec3> vertices;
	std::vector<unsigned int> faces;
MeshNode::MeshNode(MeshNode* meshNode, glm::vec3 startPos) : SceneNode(NodeType::MESH), path(path)
{
	vao = meshNode->vao;
	
	vbo = meshNode->vbo;
	ibo = meshNode->ibo;
	nbo = meshNode->nbo;
	uvbo = meshNode->uvbo;
	wbo = meshNode->wbo;
	cbo = meshNode->cbo;


	shaderProgram = meshNode->shaderProgram;

	modelMatrix = glm::translate(startPos);
	mvpLocation = meshNode->mvpLocation;		//mvp identifier for shader
}
MeshNode::MeshNode(glm::vec3 startPos) : SceneNode(NodeType::MESH), path(path)
{
	modelMatrix = glm::translate(startPos);// glm::mat4(1.f);
	shaderProgram = ShaderLoader::getInstance()->getShaderProgram("defaultShader")->getShaderId();
	mvpLocation = glGetUniformLocation(shaderProgram, "MVP");		//identify data location on shader
}

void MeshNode::draw(glm::mat4 vp)
{
	SceneNode::draw(vp);
	mvp = vp * modelMatrix;

	glUseProgram(shaderProgram);

	glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &mvp[0][0]);		//pass MVP to shader


	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glDrawElements(GL_TRIANGLES, numberOfFaces * 3, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);

}

void loadMesh()
{
	aiMesh* mesh;
	for (unsigned int j = 0; j < mesh-> mNumFaces; ++j)
	{
		vertices.push_back(glm::vec3(mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z));
	}

	for (unsigned int j = 0; j < mesh->mNumFaces; ++j)
	{
		if (mesh->mFaces[j].mNumIndices != 3) 
		faces.push_back(mesh->mFaces[j].mIndices[0]);
		faces.push_back(mesh->mFaces[j].mIndices[1]);
		faces.push_back(mesh->mFaces[j].mIndices[2]);
	}

	aiMesh* mesh = AssetLoader::getInstance()->getMesh(DUCK)->mMeshes[0];
	//Texture texture = AssetLoader::getInstance()->getMesh;
}

void MeshNode::bind()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
		);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* faces.size(), faces.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glGenBuffers(1, &nbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, nbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float)* faces.size(), faces.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glGenBuffers(1, &uvbo);
	glBindBuffer(GL_ARRAY_BUFFER, uvbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(unsigned int)* vertices.size(), faces.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	/*glGenBuffers(1, &wbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float)* faces.size(), faces.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // weiß nicht was zu weights gehört :(
	glGenBuffers(1, &cbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cbo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* faces.size(), faces.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	*/

	glBindVertexArray(0);
}
void MeshNode::update(double dT, InputHandler* input)
{
	SceneNode::update(dT, input);
}

MeshNode::~MeshNode()
{
}
