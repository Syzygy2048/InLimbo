#include "SkyboxPart.hpp"
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>		// C++ importer interface
#include <assimp/scene.h>			// Output data structure
#include <assimp/postprocess.h>		// Post processing flags


//Vertex Attribute Locations

using namespace projectname;
using namespace projectname::scene;

SkyboxPart::SkyboxPart(glm::mat4& matrix, Shader* _shader) : SceneObject(matrix), shader(_shader){
	textureID = 3;

	rigidExists = false;

	// Create an instance of the Importer class
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile( "Models/SkyBoxPart.dae",
	aiProcess_CalcTangentSpace |
	aiProcess_Triangulate |
	aiProcess_JoinIdenticalVertices |
	aiProcess_SortByPType);

	// If the import failed, report it
	if( !scene){
		std::cerr << "Failed to open SkyBoxPart.dea" << std::endl;
	}

	if( scene->HasMeshes() ) {
		for(unsigned int i = 0; i < scene->mNumMeshes; ++i){
			const struct aiMesh* mesh = scene->mMeshes[i];

			//create array with faces
			unsigned int* faceArray = (unsigned int*)malloc(sizeof(unsigned int) * mesh->mNumFaces * 3);
			unsigned int faceIndex = 0;

			for(unsigned int t = 0;  t < mesh->mNumFaces; ++t){
				const struct aiFace* face = &mesh->mFaces[t];

				memcpy(&faceArray[faceIndex], face->mIndices, 3*sizeof(float));
				faceIndex += 3;
			} 

			numFaces = scene->mMeshes[i]->mNumFaces;

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

			//unbind ALL buffer
			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER,0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);	

			//create material uniform buffer
			struct aiMaterial *mtl = scene->mMaterials[mesh->mMaterialIndex];
			
			aiString texPath = aiString("Models/SkyBoxPartUV_work.png");
			int texIndex, texCount;

			if(!AI_SUCCESS == mtl->GetTexture(aiTextureType_DIFFUSE, 0, &texPath)){
				std::cout << "Failed to load materials" << std::endl;
			}

			aiColor4D d,a,s, e;
			float shininess;

			if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &d)){
				this->diffuse[0] = d.r;
				this->diffuse[1] = d.g;
				this->diffuse[2] = d.b;
				this->diffuse[3] = d.a;
			}	

			if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &s)){
				this->specular[0] = s.r;
				this->specular[1] = s.g;
				this->specular[2] = s.b;
				this->specular[3] = s.a;
			}

			if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &a)){
				this->ambient[0] = a.r;
				this->ambient[1] = a.g;
				this->ambient[2] = a.b;
				this->ambient[3] = a.a;
			}			

			/*if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &e)){
				this->emission[0] = e.r;
				this->emission[1] = e.g;
				this->emission[2] = e.b;
				this->emission[3] = e.a;
			}*/		

			unsigned int max = 1;
			if(AI_SUCCESS == aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max)){
				this->shininess = shininess;
			}

		}
	}
}

SkyboxPart::~SkyboxPart(){

	glDeleteBuffers(1, &faceBuffer);
	glDeleteBuffers(1, &positionBuffer);
	glDeleteBuffers(1, &normalBuffer);
	glDeleteBuffers(1, &uvBuffer);
	glDeleteVertexArrays(1, &vao);
}

void SkyboxPart::update(double deltaT, int key, float speed){}

void SkyboxPart::draw(){

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, numFaces*3, GL_UNSIGNED_INT,0);
	glBindVertexArray(0);
}

btRigidBody* SkyboxPart::getRigid() {
	
	return nullptr;
}