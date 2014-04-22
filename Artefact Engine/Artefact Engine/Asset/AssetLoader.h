#pragma once

#include <GL/glew.h>

#include <unordered_map>
#include "../SceneGraph/MeshNode.h"

#include <assimp/Importer.hpp>		// C++ importer interface
#include <assimp/scene.h>			// Output data structure
#include <assimp/postprocess.h>		// Post processing flags
//Hack
#include <assimp/cimport.h>


class AssetLoader
{
public:
	const aiScene* getMesh(std::string identifier);
		
	static AssetLoader* getInstance();
	~AssetLoader();

private:
	AssetLoader();
	AssetLoader(AssetLoader const&);             
	void operator=(AssetLoader const&);

	std::unordered_map<std::string, const aiScene*> assets;
	MeshNode* meshNode;

	GLuint loadTexture(const char* imagepath);
	void loadMesh(MeshNode* node, aiMesh* mesh, const char* path);

};

