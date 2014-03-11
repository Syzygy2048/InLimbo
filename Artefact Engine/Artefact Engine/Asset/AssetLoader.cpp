#include "AssetLoader.h"

#include <GL\glfw3.h>
#include <glm\glm.hpp>


AssetLoader::AssetLoader()
{
	
	MeshNode* mesh = new MeshNode(glm::vec3(0, 0, 0));
	std::string path = "Asset//Models//duck.dae";

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	mesh->setVao(vao);
	mesh->setVertexBuffer(loadMesh(path.c_str()));
	
	glBindVertexArray(0);
}

GLuint AssetLoader::loadMesh(const char* path)
{

}	
GLuint AssetLoader::loadTexture(const char* path)
{
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glfwLoadTexture2D(path, 0); //calls glTexImage2D for us

	//Trilinear filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	return textureId; */
};


AssetLoader::~AssetLoader()
{
	delete instance;
}
