#pragma once

#include <GL/glew.h>

#include <unordered_map>
#include "../SceneGraph/MeshNode.h"

class AssetLoader
{
public:


	MeshNode* getMesh();
	
	
	static AssetLoader* getInstance()
	{
		static AssetLoader INSTANCE;
		return &INSTANCE;
	}
	~AssetLoader();

private:
	AssetLoader();
	AssetLoader(AssetLoader const&);             
	void operator=(AssetLoader const&);

	std::unordered_map<std::string, MeshNode> map;


	GLuint loadTexture(const char* imagepath);
	GLuint loadMesh(const char* path);

};

