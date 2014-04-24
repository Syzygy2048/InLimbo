#pragma once

#include <GL/glew.h>

#include <unordered_map>
#include "../SceneGraph/MeshNode.h"

#include <assimp/Importer.hpp>		// C++ importer interface
#include <assimp/scene.h>			// Output data structure
#include <assimp/postprocess.h>		// Post processing flags
#include "../Texture.h"
//Hack
#include <assimp/cimport.h>


class AssetLoader
{
public:
	const aiScene* getMesh(std::string identifier);
		
	static AssetLoader* getInstance();
	~AssetLoader();
	Texture* texture;

private:
	AssetLoader();
	AssetLoader(AssetLoader const&);             
	void operator=(AssetLoader const&);

	std::unordered_map<std::string, const aiScene*> assets;
	MeshNode* meshNode;

	void loadMesh(std::string identifyer);
	void loadTexture(std::string identifyer);
	//GLuint loadTexture(const char* imagepath);
	//void loadMesh(MeshNode* node, aiMesh* mesh, const char* path);
	

};

