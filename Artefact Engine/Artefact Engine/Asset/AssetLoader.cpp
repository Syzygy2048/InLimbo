#include "AssetLoader.h"

#include <iostream>

#include <GL\glfw3.h>
#include <glm\glm.hpp>



AssetLoader::AssetLoader()
{	
	MeshNode* node = new MeshNode(glm::vec3(0, 0, 0));
	std::string path = "Asset//Models//duck.dae";
	

	//Importer Hack
	const aiScene* aScene = aiImportFileEx(path.c_str(),
		aiProcessPreset_TargetRealtime_Quality |
		aiProcess_FindInstances |
		aiProcess_ValidateDataStructure |
		aiProcess_OptimizeMeshes,
		NULL);

	// If the import failed, report it
	if (!aScene)
	{
		std::cerr << "Failed to open" << path << std::endl;
	}
	else if (aScene->HasMeshes())
	{
		aiMesh* mesh = aScene->mMeshes[0];

		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		node->setVao(vao);
		loadMesh(node, mesh, path.c_str());

		glBindVertexArray(0);
	}
	meshNode = node;
}

AssetLoader* AssetLoader::getInstance()
{
	static AssetLoader INSTANCE;
	return &INSTANCE;
}


void AssetLoader::loadMesh(MeshNode* node, aiMesh* mesh, const char* path)
{
	if (mesh->HasPositions())
	{
		GLuint positionBuffer;
		glGenBuffers(1, &positionBuffer);

		std::vector<float> vertices;
		for (unsigned int j = 0; j < mesh->mNumFaces; ++j)
		{
			vertices.push_back(mesh->mVertices[j].x);
			vertices.push_back(mesh->mVertices[j].y);
			vertices.push_back(mesh->mVertices[j].z);
		}

		glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
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

		GLuint indexBuffer;
		glGenBuffers(1, &indexBuffer);

		std::vector<unsigned int> faces;
		for (unsigned int j = 0; j < mesh->mNumFaces; ++j)
		{
			if (mesh->mFaces[j].mNumIndices != 3) std::cerr << "well shit" << std::endl;
			faces.push_back(mesh->mFaces[j].mIndices[0]);
			faces.push_back(mesh->mFaces[j].mIndices[1]);
			faces.push_back(mesh->mFaces[j].mIndices[2]);
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * faces.size(), faces.data(), GL_STATIC_DRAW);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, g_index_buffer_data, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


		node->setVertexBuffer(positionBuffer);
		node->setIndexBuffer(indexBuffer);
		//node->setNumberOfFaces(2);
		node->setNumberOfFaces(mesh->mNumFaces);
	}





}	


MeshNode* AssetLoader::getMesh()
{
	return meshNode;
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
