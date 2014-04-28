//#include <GL\glfw3.h>

#include <GL/glew.h>
#include <glm\glm.hpp>

#include <PxToolkit\PxToolkit.h>

#include "MazeTile.h"
#include "MazeGenerator.h"
#include "../Enums/MazeEnums.h"
#include "../Asset/AssetLoader.h"
#include "../Enums/MeshPaths.h"

#include "../Shaders/ShaderLoader.h"


MazeTile::MazeTile() : SceneNode(MESH)
{
	tile = MazeGenerator(TILE_SIZE).generate();

	shaderProgram = ShaderLoader::getInstance()->getShaderProgram("textureShader")->getShaderId();
	mvpLocation = glGetUniformLocation(shaderProgram, "MVP");
}


void MazeTile::mergeAsMesh()
{
	//put the maze mesh together
	aiMesh* mesh = AssetLoader::getInstance()->getMesh(CUBE)->mMeshes[0];
	aiMesh* floorMesh = AssetLoader::getInstance()->getMesh(FLOOR)->mMeshes[0];
	for (int i = 0; i < TILE_SIZE; i++)
	{
		for (int j = 0; j < TILE_SIZE; j++)
		{
			unsigned int index = vertices.size();
			if (tile[j + i * TILE_SIZE] == MAZE_WALL)
			{
				
				for (int k = 0; k < mesh->mNumVertices; k++)
				{
					vertices.push_back(
						glm::vec3(
						mesh->mVertices[k].x - TILE_SIZE / 2 + j *2,
						mesh->mVertices[k].y,
						mesh->mVertices[k].z - TILE_SIZE / 2 + i *2));
				}
				for (int k = 0; k < mesh->mNumFaces; k++)
				{
					faces.push_back(mesh->mFaces[k].mIndices[0] + index);
					faces.push_back(mesh->mFaces[k].mIndices[1] + index);
					faces.push_back(mesh->mFaces[k].mIndices[2] + index);
				}
			}
			else if (tile[j + i * TILE_SIZE] == MAZE_HALLWAY || tile[j + i * TILE_SIZE] == REMOVED_BLOCK)
			{
				for (int k = 0; k < floorMesh->mNumVertices; k++)
				{
					vertices.push_back(
						glm::vec3(
						floorMesh->mVertices[k].x - TILE_SIZE / 2 + j * 2,
						floorMesh->mVertices[k].y,
						floorMesh->mVertices[k].z - TILE_SIZE / 2 + i * 2));
				}
				for (int k = 0; k < floorMesh->mNumFaces; k++)
				{
					faces.push_back(floorMesh->mFaces[k].mIndices[0] + index);
					faces.push_back(floorMesh->mFaces[k].mIndices[1] + index);
					faces.push_back(floorMesh->mFaces[k].mIndices[2] + index);
				}
			}
		}
	}


	//start of mesh merge algorithm

	std::cerr << "vertices before merge " << vertices.size() << std::endl;
	std::unordered_map<float, unsigned int> map;
	std::vector<glm::vec3> newVertices;
	std::vector<unsigned int> vertShift(vertices.size());
	for (int i = 0; i < vertices.size(); i++)
	{
		//round x, y, z to nearest number with 3 decimals e.g. 0.9999 = 1, 0.999 = 0.999
		vertices[i].x = round(vertices[i].x * 1000) / 1000;
		vertices[i].y = round(vertices[i].y * 1000) / 1000;
		vertices[i].z = round(vertices[i].z * 1000) / 1000;

		//hash it with something like x*prime1 + y*prime2 + z*prime3, times 1000 to remove float inaccuracies
		float hash = vertices[i].x * 1000 * 11311 + vertices[i].y * 1000 * 1114111 + vertices[i].z * 1000 * 111181111; //hardcoded numbers are primes
				
		if(map.count(hash) == 0)
		{
			map[hash] = newVertices.size();
			newVertices.push_back(vertices[i]);
		}
		vertShift[i] = map[hash];
	}
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		faces[i] = vertShift[faces[i]];
	}
	
	vertices = newVertices;
	
	std::cerr << "vertices after merge " << vertices.size() << std::endl;
	
	std::cerr << "faces before merge " << faces.size() << std::endl;
	//remove duplicate faces
	//check all faces (each face has 3 elements in the faces vector) if they exist twice (in any combination),
	map.clear();
	std::unordered_map<float, std::vector<unsigned int>> map2;
	for (unsigned int i = 0; i < faces.size(); i += 3)
	{
		unsigned int a = faces[i];
		unsigned int b = faces[i+1];
		unsigned int c = faces[i+2];		
		if (b > a)					//order by largest first for easier hashing
		{
			unsigned int temp = a;
			a = b;
			b = temp;
		}
		if (c > a)
		{
			unsigned int temp = a;
			a = c;
			c = temp;
		}
		if (c > b)
		{
			unsigned int temp = b;
			b = c;
			c = temp;
		}

		float hash = a * 11311 + b * 1114111 + c * 111181111;
		if (map2.count(hash) == 0)
		{
			std::vector<unsigned int> face(3);
			
			face[0] = faces[i];
			face[1] = faces[i + 1];
			face[2] = faces[i + 2];
			map2[hash] = face;
		}
		else //remove both duplicates (since a duplicate face means both sides are in a wall and out of always out of view) in our case, just add no face instead
		{
			map2.erase(hash);
		}
	}

	faces.clear();

	for (std::pair<float, std::vector<unsigned int>> pair : map2)
	{
		for (unsigned int verticeId : pair.second)
		{
			faces.push_back(verticeId);
		}
	} 
	std::cerr << "faces after merge " << faces.size() << std::endl; 

	/*texture = AssetLoader::getInstance()->getTexture(CUBE);
	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		uvCoords.push_back(glm::vec2(mesh->mTextureCoords[0]->x, mesh->mTextureCoords[0]->y));
	}*/
}

void MazeTile::bind()
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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* faces.size(), faces.data(), GL_STATIC_DRAW);



	glGenBuffers(1, &uvbo);
	glBindBuffer(GL_ARRAY_BUFFER, uvbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * uvCoords.size(), uvCoords.data(), GL_STATIC_DRAW);
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




	glBindVertexArray(0);
}

void MazeTile::createCollisionShape(physx::PxScene* scene, physx::PxPhysics* physicsSDK, physx::PxCooking* cooking)
{
	physx::PxRigidStatic* meshActor = physicsSDK->createRigidStatic(physx::PxTransform());
	physx::PxTriangleMeshDesc meshDesc;
	meshDesc.points.count = vertices.size();
	meshDesc.points.stride = sizeof(glm::vec3);
	meshDesc.points.data = vertices.data();

	meshDesc.triangles.count = faces.size();
	meshDesc.triangles.stride = 3*sizeof(unsigned int);
	meshDesc.triangles.data = faces.data();

	//PxToolkit::createTriangleMesh32(physicsSDK, cooking, &meshDesc);
	PxToolkit::PxDefaultMemoryOutputStream writeBuffer;
	cooking->cookTriangleMesh(meshDesc, writeBuffer);
	PxToolkit::PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
	physx::PxTriangleMesh* triangleMesh = physicsSDK->createTriangleMesh(readBuffer);
	physx::PxShape* triangleMeshShape = meshActor->createShape(physx::PxTriangleMeshGeometry(triangleMesh), *physicsSDK->createMaterial(0.5f, 0.5f, 0.1f), physx::PxShapeFlag::eSCENE_QUERY_SHAPE);
	scene->addActor(*meshActor);	//create shape creates and adds the shape to the actor, so it does not have to be bound again;


}


void MazeTile::update(double dT, InputHandler* input)
{
	SceneNode::update(dT, input);
}

void MazeTile::draw(glm::mat4 vp)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	SceneNode::draw(vp);
	mvp = vp * modelMatrix;
	glUseProgram(shaderProgram);

	glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &mvp[0][0]);		//pass MVP to shader


	glBindVertexArray(vao);
	glBindTexture(GL_TEXTURE_2D, texbo);
	glDrawElements(GL_TRIANGLES, faces.size(), GL_UNSIGNED_INT, (void*)0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void MazeTile::setNeighbor(MazeTile* neighbor, int relativePosition)
{
	if (relativePosition = 0)
		top = neighbor;
	else if (relativePosition = 1)
		topRight = neighbor;
	else if (relativePosition = 2)
		right = neighbor;
	else if (relativePosition = 3)
		bottomRight = neighbor;
	else if (relativePosition = 4)
		bottom = neighbor;
	else if (relativePosition = 5)
		bottomLeft = neighbor;
	else if (relativePosition = 6)
		left = neighbor;
	else if (relativePosition = 7)
		topLeft = neighbor;
}


MazeTile::~MazeTile()
{
	delete[] tile;
}