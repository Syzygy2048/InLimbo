#include "AssetLoader.h"

#include <iostream>

#include <GL\glfw3.h>
#include <glm\glm.hpp>

#include "../Enums/MeshPaths.h"

AssetLoader::AssetLoader()
{
}

AssetLoader* AssetLoader::getInstance()
{
	static AssetLoader INSTANCE;
	return &INSTANCE;
}

void AssetLoader::loadMesh(std::string identifyer)
{
	//Importer Hack
	const aiScene* aScene = aiImportFileEx(identifyer.c_str(),
		aiProcessPreset_TargetRealtime_Quality |
		aiProcess_JoinIdenticalVertices |
		aiProcess_FindInstances |
		aiProcess_ValidateDataStructure |
		aiProcess_OptimizeMeshes |
		aiProcess_Triangulate,
		NULL);

	// If the import failed, report it
	if (!aScene)
	{
		std::cerr << "Failed to open" << identifyer << std::endl;
	}
	else if (aScene->HasMeshes())
	{
		aiMesh* mesh = aScene->mMeshes[0];
		assets.insert(std::pair<std::string, const aiScene*>(identifyer, aScene));
	}
}
const aiScene* AssetLoader::getMesh(std::string identifier)
{
	if (assets.count(identifier) == 0)
	{
		loadMesh(identifier);
		assets.find(identifier)->second;
	}
	return assets.find(identifier)->second;
}





/*GLuint AssetLoader::loadTexture(const char* path)
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

	return textureId; 
}; */


AssetLoader::~AssetLoader()
{

}
