#include "AssetLoader.h"

#include <iostream>

#include <GL\glfw3.h>
#include <glm\glm.hpp>

#include "../Enums/MeshPaths.h"

AssetLoader::AssetLoader()
{
}

AssetLoader* AssetLoader::getInstance()
{
	static AssetLoader INSTANCE;
	return &INSTANCE;
}

void AssetLoader::loadMesh(std::string identifyer)
{
	//Importer Hack
	const aiScene* aScene = aiImportFileEx(identifyer.c_str(),
		aiProcessPreset_TargetRealtime_Quality |
		aiProcess_JoinIdenticalVertices |
		aiProcess_FindInstances |
		aiProcess_ValidateDataStructure |
		aiProcess_OptimizeMeshes |
		aiProcess_Triangulate,
		NULL);

	// If the import failed, report it
	if (!aScene)
	{
		std::cerr << "Failed to open" << identifyer << std::endl;
	}
	else if (aScene->HasMeshes())
	{
		aiMesh* mesh = aScene->mMeshes[0];
		assets.insert(std::pair<std::string, const aiScene*>(identifyer, aScene));
	}
}
const aiScene* AssetLoader::getMesh(std::string identifier)
{
	if (assets.count(identifier) == 0)
	{
		loadMesh(identifier);
	}
	return assets.find(identifier)->second;
}

FIBITMAP* AssetLoader::getTexture(std::string identifier)
{
	if (textures.count(identifier) == 0)
	{
		loadTexture(identifier);
	}
	return textures.find(identifier)->second;
}

void AssetLoader::loadTexture(std::string identifyer)
{
	const char* texturePath;

	if (identifyer == DUCK)
	{
		 texturePath = DUCK_TEXTURE.c_str();
	}
	else if (identifyer == COW)
	{
		texturePath = COW_TEXTURE.c_str();
	}
	else if (identifyer == CUBE)
	{
		texturePath = CUBE_TEXTURE.c_str();
	}
	else
	{
		texturePath = "texture not yet implemented";
		//TODO: think of a better system than this, maybe a map that matches each texture to it's texture, maybe store texture path in aiScene if possible
	}
	
	FIBITMAP* image;
	
	try
	{
		FREE_IMAGE_FORMAT format = FreeImage_GetFIFFromFilename(identifyer.c_str()); //detection of format

		std::cerr << "format: " << format << std::endl;
		//image = FreeImage_Load(format, texturePath);
		image = FreeImage_Load(FIF_PNG, texturePath);		//TODO: probably a bad idea to hardcode .png as the filetype of the texture file here.
		image = FreeImage_ConvertTo32Bits(image);

	}
	catch (int e)
	{
		std::cerr << "Couldn't load Texture: " << texturePath << e << std::endl;
		system("PAUSE");
		exit(-1);
	}

	if (!image)
	{
		std::cerr << "Couldn't load Texture: " << texturePath << std::endl;
		system("PAUSE");
		exit(-1);
	}

	BYTE *pixels = (BYTE*)FreeImage_GetBits(image);
	unsigned int width = FreeImage_GetWidth(image);
	unsigned int height = FreeImage_GetHeight(image);

	// if something went wrong, exit with error
	if (pixels == NULL || width == 0 || height == 0){
		std::cerr << "Error loading Texture: " << texturePath 
			<< "\n width: " << width 
			<< "\n height" << height
			<< "\n pixeldata is null: " << (pixels == NULL) << std::endl;
		system("PAUSE");
		exit(-1);
	}
	textures[identifyer] = image;
}


AssetLoader::~AssetLoader()
{

}
