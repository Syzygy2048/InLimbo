#include "Monster.hpp"
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

using namespace projectname;
using namespace projectname::scene;

//Spin
int spin = 10;
float leftRight = 0.0f;
float upDown = 0.0f;
float forBack = 0.0f;
float turn = 0.0;

double myTime = 0;

btConvexHullShape* Astronaut::astronautCollShape = new btConvexHullShape();
bool Astronaut::setAstronautColl =  false;
btScalar Astronaut::astronautmass = 1.0f;
btVector3 Astronaut::astronautinertia(0,0,0);

long g_lLastTime = 0;
long g_lElapsedTime = 0;


Astronaut::Astronaut(glm::mat4& matrix, Shader* _shader) : SceneObject(matrix), shader(_shader){
	initMatrix = matrix;
	textureID = 0;

	// Create an instance of the Importer class
	/*Assimp::Importer* importer = new Assimp::Importer();
	aScene = importer->ReadFile( "Models/AstronautTextureBones.dae", 
		aiProcess_CalcTangentSpace | 
		aiProcess_Triangulate | 
		aiProcess_JoinIdenticalVertices | 
		aiProcess_SortByPType);*/

	//Importer Hack
	aScene = aiImportFileEx("Models/AstronautTextureBones.dae",
	aiProcessPreset_TargetRealtime_Quality |
		aiProcess_FindInstances | 
		aiProcess_ValidateDataStructure | 
		aiProcess_OptimizeMeshes, 
	NULL);

	// If the import failed, report it
	if( !aScene){
		std::cerr << "Failed to open Monster.dea" << std::endl;
	}

	monsterCollShape = new btConvexHullShape();
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

			if(mesh->HasBones()){

				//BONES----------------------------------------------------------------------------------------------------------------------------

				//read bone indices and weights for bone animation
				bStruct = new BoneStructure();

				bStruct->bsBoneIndices = new glm::vec4[mesh->mNumVertices];
				bStruct->bsWeights = new glm::vec4[mesh->mNumVertices];

				std::vector<aiVertexWeight> * vTempWeightsPerVertex = new std::vector<aiVertexWeight>[mesh->mNumVertices];

				for(int i = 0; i < mesh->mNumVertices; i++)vTempWeightsPerVertex[i].clear();

				for (unsigned int j = 0; j < mesh->mNumBones; j++) {
					const aiBone* pBone = mesh->mBones[j];

					for (unsigned int b = 0; b < pBone->mNumWeights; b++) {
						vTempWeightsPerVertex[pBone->mWeights[b].mVertexId].push_back(aiVertexWeight(j, pBone->mWeights[b].mWeight));
					}
				}

				for (int j = 0; j < mesh->mNumVertices; j++) {
					bStruct->bsBoneIndices[j] = glm::uvec4(0, 0, 0, 0);
					bStruct->bsWeights[j] = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

					if (mesh->HasBones()) {
						if (vTempWeightsPerVertex[j].size() > 4) {

						std::cerr << "The model has invalid bone weights and is not loaded." << std::endl;
						system("PAUSE");
						exit(-1);
						}

						//float sum = 0.0f;
						for (unsigned int k = 0; k < vTempWeightsPerVertex[j].size(); k++) {
							bStruct->bsBoneIndices[j][k] = (GLfloat) vTempWeightsPerVertex[j][k].mVertexId;
							bStruct->bsWeights[j][k] = (GLfloat) vTempWeightsPerVertex[j][k].mWeight;

							//sum += vTempWeightsPerVertex[j][k].mWeight;
						}

						//std::cerr << sum << std::endl;
					}
				}

				if (vTempWeightsPerVertex != NULL) {
					delete[] vTempWeightsPerVertex;
					vTempWeightsPerVertex = NULL;
				}

				glGenBuffers(1, &weightBuffer);
				glBindBuffer(GL_ARRAY_BUFFER, weightBuffer);
				GLint weightIndex = glGetAttribLocation(shader->programHandle, "inBoneWeights");
				glBufferData(GL_ARRAY_BUFFER, sizeof(float)*4*mesh->mNumVertices, bStruct->bsWeights, GL_STATIC_DRAW);
				glEnableVertexAttribArray(weightIndex);
				glVertexAttribPointer(weightIndex,4, GL_FLOAT, GL_FALSE, 0, 0);

				glGenBuffers(1, &indexBuffer);
				glBindBuffer(GL_ARRAY_BUFFER, indexBuffer);
				GLint idicesIndex = glGetAttribLocation(shader->programHandle, "inBoneIndices");
				glBufferData(GL_ARRAY_BUFFER, sizeof(float)*4*mesh->mNumVertices, bStruct->bsBoneIndices, GL_STATIC_DRAW);
				glEnableVertexAttribArray(idicesIndex);
				glVertexAttribPointer(idicesIndex, 4, GL_FLOAT, GL_FALSE, 0, 0);

			}			

			//unbind ALL buffer
			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER,0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);	

			//create material uniform buffer
			struct aiMaterial *mtl = aScene->mMaterials[mesh->mMaterialIndex];

			aiString texPath = aiString("Models/AstronautUV_work.jpg");
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

		animator = NULL;
		if (aScene->HasAnimations()) {
			animator = new Animator(aScene, 0);
			animator->SetAnimationIndex(0);

			animator->UpdateAnimation(g_lElapsedTime, 1);
		}
		else animator = NULL;

		g_lLastTime = glfwGetTime();
	}

	astronautCollShape->setLocalScaling(btVector3(0.002f,0.002f,0.002f));
	astronautCollShape->calculateLocalInertia(astronautmass, astronautinertia);

	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(btVector3(0,0,0));

	btDefaultMotionState* motionState = new btDefaultMotionState(trans);
	btRigidBody::btRigidBodyConstructionInfo astronautrigidCI(astronautmass, motionState, astronautCollShape, astronautinertia);
	astronautrigid = new btRigidBody(astronautrigidCI);
	rigidExists = true;
}

Astronaut::~Astronaut(){

	glDeleteBuffers(1, &faceBuffer);
	glDeleteBuffers(1, &positionBuffer);
	glDeleteBuffers(1, &normalBuffer);
	glDeleteBuffers(1, &uvBuffer);
	glDeleteBuffers(1, &weightBuffer);
	glDeleteBuffers(1, &indexBuffer);
	glDeleteVertexArrays(1, &vao);
}

void Astronaut::update(double deltaT, int key, float speed){

	//Rotate - Spin happens always
	glm::vec3 rotation = glm::vec3(0.0f, 1.0f, 0.0f);
	float aspect = spin * (float) deltaT;
	
	modelMatrix = glm::rotate(modelMatrix, aspect, rotation);
	glm::mat4 lastPosition = modelMatrix;

	turn += aspect;
	

	//astronautrigid->setLinearFactor(btVector3(0.0f,0.0f,0.0f));

	if(key == 1) { //left
		if(leftRight < 166){
			leftRight += deltaT*100;
			//astronautrigid->setLinearFactor(btVector3(1.0f,0.0f,0.0f));
			modelMatrix = glm::translate(initMatrix, glm::vec3(leftRight, forBack, -upDown));
			modelMatrix = glm::rotate(modelMatrix, turn, rotation);
		}
	}

	else if(key == 2) { //right
		if(leftRight > -166){
			leftRight -= deltaT*100;
			modelMatrix = glm::translate(initMatrix, glm::vec3(leftRight, forBack, -upDown));
			modelMatrix = glm::rotate(modelMatrix, turn, rotation);
		}
	}

	else if(key == 3) { //up
		if(upDown < 186){
			upDown += deltaT*100;
			modelMatrix = glm::translate(initMatrix, glm::vec3(leftRight, forBack,-upDown));
			modelMatrix = glm::rotate(modelMatrix, turn, rotation);
		}
	}

	else if(key == 4) { //down
		if(upDown > -126){
			upDown -= deltaT*100;
			modelMatrix = glm::translate(initMatrix, glm::vec3(leftRight, forBack,-upDown));
			modelMatrix = glm::rotate(modelMatrix, turn, rotation);
		}
	}

	else if(key == 5) { //accelerate
		if(spin != 500)	//speed limit is 100
			++spin;	

		if(forBack < 300) {
			forBack += deltaT*150;
			upDown -= deltaT*75;
			//forBack += 2*pow(10, deltaT);
			modelMatrix = glm::translate(initMatrix, glm::vec3(leftRight, forBack, -upDown));
			modelMatrix = glm::rotate(modelMatrix, turn, rotation);
		}
	}
	else if(key == 6) { //slow down
		if(spin != 10)	//can not be negative
			--spin;

		if(forBack >= 0) {
			forBack -= deltaT*150;
			upDown += deltaT*75;
			modelMatrix = glm::translate(initMatrix, glm::vec3(leftRight, forBack, -upDown));
			modelMatrix = glm::rotate(modelMatrix, turn, rotation);
		}
	}
	else { //automatically goes back to the origin position on the z-axis
		if(forBack >= 10) {
			forBack -= deltaT*100;
			upDown += deltaT*50;
			modelMatrix = glm::translate(initMatrix, glm::vec3(leftRight, forBack, -upDown));
			modelMatrix = glm::rotate(modelMatrix, turn, rotation);
		}
	}
	/*
	float degree = spin * (deltaT);
	//++spin;
	float pi = 3.14159265;
	float radian = degree * (pi/180);
	//astronautrigid->setAngularFactor(btVector3(0,1,1));
	astronautrigid->setAngularVelocity(btVector3(radian, 0.0f, 0.0f));


	if(astronautRotation <= 360.0f) {
		astronautRotation += degree;
	}
	else {
		astronautRotation = degree;
	}

	
	astronautrigid->setLinearVelocity(btVector3(leftRight, forBack, -upDown));
	btVector3 t = astronautrigid->getWorldTransform().getOrigin();

	modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.002f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3((float)t.getX(), (float)t.getY(), (float)t.getZ()));
	modelMatrix = glm::rotate(glm::rotate(modelMatrix, 65.0f, glm::vec3(-1.0f,0.0f,0.0f)), astronautRotation, glm::vec3(0.0f,1.0f,0.0f));
	*/
}

unsigned long startTime = 0L;

void Astronaut::draw(){
	//set the bone animation to the specified timestamp
	if (animator != NULL) {
		//set animation-flag
		//glUniform1i(glGetUniformLocation(shader->programHandle, "animate"), true);

		if(startTime == 0)
			startTime = clock();//glfwGetTime();

		g_lElapsedTime = clock() - startTime;
		animator->UpdateAnimation(g_lElapsedTime, 0.5);		//Backflip-> 1000000 
															//Frontflip-> 0.5
		//set shader uniforms
		glUniformMatrix4fv(glGetUniformLocation(shader->programHandle, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

		glBindVertexArray(vao);

		//draw the model
		if (aScene->mRootNode != NULL) {
			RenderNode(aScene->mRootNode);
		}
	}
	
	glDrawElements(GL_TRIANGLES, numFaces*3, GL_UNSIGNED_INT,0);
	glBindVertexArray(0);

	//glUniform1i(glGetUniformLocation(shader->programHandle, "animate"), false);
}

void Astronaut::RenderNode(aiNode * pNode) {
	int maxBonesPerMesh = 60;
	for (unsigned int i = 0; i < pNode->mNumMeshes; i++) {
		const aiMesh* astronautMesh = aScene->mMeshes[pNode->mMeshes[i]];
		glm::mat4* pMatrices = new glm::mat4[maxBonesPerMesh];

		//upload bone matrices
		if ((astronautMesh->HasBones()) && (animator != NULL)) {
			const std::vector<aiMatrix4x4>& vBoneMatrices = animator->GetBoneMatrices(pNode, i);

			for (unsigned int j = 0; j < astronautMesh->mNumBones; j++) {
				if (j < maxBonesPerMesh) {
					pMatrices[j][0][0] = vBoneMatrices[j].a1;
					pMatrices[j][0][1] = vBoneMatrices[j].b1;
					pMatrices[j][0][2] = vBoneMatrices[j].c1;
					pMatrices[j][0][3] = vBoneMatrices[j].d1;
					pMatrices[j][1][0] = vBoneMatrices[j].a2;
					pMatrices[j][1][1] = vBoneMatrices[j].b2;
					pMatrices[j][1][2] = vBoneMatrices[j].c2;
					pMatrices[j][1][3] = vBoneMatrices[j].d2;
					pMatrices[j][2][0] = vBoneMatrices[j].a3;
					pMatrices[j][2][1] = vBoneMatrices[j].b3;
					pMatrices[j][2][2] = vBoneMatrices[j].c3;
					pMatrices[j][2][3] = vBoneMatrices[j].d3;
					pMatrices[j][3][0] = vBoneMatrices[j].a4;
					pMatrices[j][3][1] = vBoneMatrices[j].b4;
					pMatrices[j][3][2] = vBoneMatrices[j].c4;
					pMatrices[j][3][3] = vBoneMatrices[j].d4;
				}
			}
		}

		//upload the complete bone matrices to the shaders
		glUniformMatrix4fv(glGetUniformLocation(shader->programHandle,"boneMatrices"), maxBonesPerMesh,GL_FALSE, glm::value_ptr(*pMatrices));

		delete pMatrices;
	}

	//render all child nodes
	for (unsigned int i = 0; i < pNode->mNumChildren; i++) {
		RenderNode(pNode->mChildren[i]);
	}
}

void Astronaut::initializeBullet(float x, float y, float z, btDiscreteDynamicsWorld* world) {

	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(btVector3(x,y,z));
	trans.setRotation(btQuaternion(btVector3(0.0f,1.0f,0.0f), 3.14159));
	trans.setRotation(btQuaternion(btVector3(1.0f,0.0f,0.0f), 1.13446));

	btDefaultMotionState* motionState = new btDefaultMotionState(trans);
	btRigidBody::btRigidBodyConstructionInfo astronautrigidCI(Astronaut::astronautmass, motionState, Astronaut::astronautCollShape, Astronaut::astronautinertia);

	astronautrigid = new btRigidBody(astronautrigidCI);
	rigidExists = true;
	world->addRigidBody(astronautrigid, COL_ASTRONAUT, astronautCollidesWith);
}

btRigidBody* Astronaut::getRigid() {
	return astronautrigid;
}
