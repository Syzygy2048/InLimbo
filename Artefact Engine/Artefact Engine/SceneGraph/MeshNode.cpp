

#include <GL\glew.h>

#include "MeshNode.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "../Shaders/ShaderLoader.h"

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

void MeshNode::update(double dT, InputHandler* input)
{
	SceneNode::update(dT, input);
}

MeshNode::~MeshNode()
{
}
