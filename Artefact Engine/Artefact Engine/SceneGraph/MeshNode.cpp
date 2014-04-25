

#include <GL\glew.h>

#include "MeshNode.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "../Enums/MazeEnums.h"
#include "../Asset/AssetLoader.h"
#include "../Enums/MeshPaths.h"



#include "../Shaders/ShaderLoader.h"

MeshNode::MeshNode(MeshNode* meshNode, glm::vec3 startPos) : SceneNode(NodeType::MESH), path(path)
{
	vao = meshNode->vao;
	
	vbo = meshNode->vbo;
	ibo = meshNode->ibo;
	nbo = meshNode->nbo;
	uvbo = meshNode->uvbo;
	wbo = meshNode->wbo;
	cbo = meshNode->cbo;


	shaderProgram = meshNode->shaderProgram;

	modelMatrix = glm::translate(startPos);
	mvpLocation = meshNode->mvpLocation;		//mvp identifier for shader
}
MeshNode::MeshNode(glm::vec3 startPos) : SceneNode(NodeType::MESH), path(path)
{
	modelMatrix = glm::translate(startPos);// glm::mat4(1.f);
}


void MeshNode::initializeMeshNode(std::string identifyer)
{
	aiMesh* mesh = AssetLoader::getInstance()->getMesh(identifyer)->mMeshes[0];
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		vertices.push_back(glm::vec3(
			mesh->mVertices[i].x,
			mesh->mVertices[i].y,
			mesh->mVertices[i].z));
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		if (mesh->mFaces[i].mNumIndices == 3) 
		{
			faces.push_back(mesh->mFaces[i].mIndices[0]);
			faces.push_back(mesh->mFaces[i].mIndices[1]);
			faces.push_back(mesh->mFaces[i].mIndices[2]);
		}
	}
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		uvs.push_back(glm::vec2(
			mesh->mTextureCoords[0][i].x,
			mesh->mTextureCoords[0][i].y));
		//uvs.push_back(mesh->mTextureCoords[0][i].x);
		//uvs.push_back(mesh->mTextureCoords[0][i].y);
	}

	
	shaderProgram = ShaderLoader::getInstance()->getShaderProgram("textureShader")->getShaderId();	//shader type should come from the aiScenes material instead
	mvpLocation = glGetUniformLocation(shaderProgram, "MVP");		//identify data location on shader
	
	texture = AssetLoader::getInstance()->getTexture(identifyer);
	//aiMesh* mesh = AssetLoader::getInstance()->getMesh(DUCK)->mMeshes[0];
	//Texture texture = AssetLoader::getInstance()->getMesh;
}

void MeshNode::bind()
{
	
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
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

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * faces.size(), faces.data(), GL_STATIC_DRAW);

	/*glGenBuffers(1, &nbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, nbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float)* faces.size(), faces.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); */
	
	
	
	
	glGenBuffers(1, &uvbo);
	glBindBuffer(GL_ARRAY_BUFFER, uvbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * uvs.size(), uvs.data(), GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(unsigned int) * uvs.size(), uvs.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
		);
	
	glGenTextures(1, &texbo);
	glBindTexture(GL_TEXTURE_2D, texbo);
	glActiveTexture(GL_TEXTURE0); // this lets you set several textures per mesh, up to at least 80. it's also used for normal maps, reflection maps etc

	unsigned int width, height;
	BYTE* pixels = (BYTE*)FreeImage_GetBits(texture);
	width = FreeImage_GetWidth(texture);
	height = FreeImage_GetHeight(texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8/*intensity*/, width, height, 0, GL_BGRA/*luminance!!*/, GL_UNSIGNED_BYTE, pixels);
	//Trilinear filtering for texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D); 
	//glBindTexture(GL_TEXTURE_2D, 0);



	/*glGenBuffers(1, &wbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float)* faces.size(), faces.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // wei� nicht was zu weights geh�rt :(
	glGenBuffers(1, &cbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cbo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* faces.size(), faces.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	*/
	
	glBindVertexArray(0);
}


void MeshNode::update(double dT, InputHandler* input)
{
	SceneNode::update(dT, input);
}


void MeshNode::draw(glm::mat4 vp)
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	SceneNode::draw(vp);
	mvp = vp * modelMatrix;

	glUseProgram(shaderProgram);

	glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &mvp[0][0]);		//pass MVP to shader
	
	glBindVertexArray(vao);
	glBindTexture(GL_TEXTURE_2D, texbo); //TODO: find a way to move this into VAO, if this is missing all meshes that have a texture will use the same texture
	glDrawElements(GL_TRIANGLES, faces.size(), GL_UNSIGNED_INT, (void*)0);
	glBindTexture(GL_TEXTURE_2D, 0); //TODO: find a way to move this into VAO
	glBindVertexArray(0);

}


MeshNode::~MeshNode()
{
}
