#pragma once

#include <GL\glew.h>
#include <iostream>
//#include "Shader"

	class AssetLoader
	{

		public:
			AssetLoader();
			GLuint loadAsset(std::string path);
			virtual ~AssetLoader();

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

