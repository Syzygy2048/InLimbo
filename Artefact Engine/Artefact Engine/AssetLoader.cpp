#include "AssetLoader.h"
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>		// C++ importer interface
#include <assimp/scene.h>			// Output data structure
#include <assimp/postprocess.h>		// Post processing flags
//Hack
#include <assimp/cimport.h>

#include <GL/glfw.h>
#include "../Shader.hpp"
#include <glm/gtc/type_ptr.hpp>



/*
btConvexHullShape* model::modelCollShape = new btConvexHullShape();
bool model::setModelcol =  false;
btScalar Model::modelmass = 1.0f;
btVector3 Model::modelinertia(0,0,0);

long g_lLastTime = 0;
long g_lElapsedTime = 0;
*/

AssetLoader::AssetLoader(std::string path){
	

	//Importer Hack
	aScene = aiImportFileEx("file",
		aiProcessPreset_TargetRealtime_Quality |
		aiProcess_FindInstances | 
		aiProcess_ValidateDataStructure | 
		aiProcess_OptimizeMeshes, 
	NULL);

	// If the import failed, report it
	if( !aScene){
		std::cerr << "Failed to open" << path << std::endl;
	}

	modelCollShape = new btConvexHullShape();
	btScalar monstermass = 1;
	btVector3 monsterinertia(0,0,0);

	if( aScene->HasMeshes() ) {
		for(unsigned int i = 0; i < aScene->mNumMeshes; ++i){
			mesh = aScene->mMeshes[i];

			//create array with faces
			unsigned int* faceArray = (unsigned int*)malloc(sizeof(unsigned int) * mesh->mNumFaces * 3);
			unsigned int faceIndex = 0;

			for(unsigned int t = 0;  t < mesh->mNumFaces; ++t){
				const struct aiFace* face = &mesh->mFaces[t];

				memcpy(&faceArray[faceIndex], face->mIndices, 3*sizeof(float));
				faceIndex += 3;
			} 

			numFaces = aScene->mMeshes[i]->mNumFaces;

			//generate Vertex Array for mesh
			glGenVertexArrays(1,&vao);
			glBindVertexArray(vao);

			//buffer for faces
			glGenBuffers(1, &faceBuffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceBuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh->mNumFaces * 3, faceArray, GL_STATIC_DRAW);

			//buffer for vertex positions
			if(mesh->HasPositions()){
				glGenBuffers(1, &positionBuffer);
				glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
				GLint positionIndex = glGetAttribLocation(shader->programHandle, "position");
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3* mesh->mNumVertices, mesh->mVertices, GL_STATIC_DRAW);
				glEnableVertexAttribArray(positionIndex);
				glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);
			}

			//buffer for vertex normals
			if(mesh->HasNormals()){
				glGenBuffers(1, &normalBuffer);
				glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
				GLint normalIndex = glGetAttribLocation(shader->programHandle, "normal");
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3* mesh->mNumVertices, mesh->mNormals, GL_STATIC_DRAW);
				glEnableVertexAttribArray(normalIndex);
				glVertexAttribPointer(normalIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);
			}

			//buffer for texture coordinats
			if(mesh->HasTextureCoords(0)){
				float *texCoords = (float*)malloc(sizeof(float)*2*mesh->mNumVertices);
				for(unsigned int k = 0; k < mesh->mNumVertices; ++k){
					texCoords[k*2]  = mesh->mTextureCoords[0][k].x;
					texCoords[k*2+1] = mesh->mTextureCoords[0][k].y;
			}			

				glGenBuffers(1, &uvBuffer);
				glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
				GLint uvIndex = glGetAttribLocation(shader->programHandle, "uv");
				glBufferData(GL_ARRAY_BUFFER, sizeof(float)*2*mesh->mNumVertices, texCoords, GL_STATIC_DRAW);
				glEnableVertexAttribArray(uvIndex);
				glVertexAttribPointer(uvIndex, 2, GL_FLOAT, GL_FALSE, 0, 0);
			}
		}
	}
