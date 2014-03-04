#pragma once

#include <iostream>
#include "../SceneGraph/MeshNode.h"

//#include "Shader"

class AssetLoader
{
public:
	AssetLoader();
	void loadAssets(std::string path, MeshNode* node);
	~AssetLoader();

private:
	GLuint vao;
	GLuint positionBuffer, normalBuffer, faceBuffer, uvBuffer, weightBuffer, indexBuffer;
	//Shader* shader;
	
	//Lightning
	GLuint diffuseBuffer, ambientBuffer ,specularBuffer, emissionBuffer, shininessBuffer;

	//Animation
	/*BoneStructure* bStruct;
	const aiScene* aScene;
	const struct aiMesh* mesh;*/

};

