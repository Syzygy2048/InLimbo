#include "Texture.h"
#include <FreeImage\FreeImage.h>
#include <iostream>

Texture::Texture(const std::string& path)
{
	FreeImage_Initialise(TRUE);
	
	glGenTextures(1, &handle);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, handle);	// type of texture 

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Load Image sizes
	unsigned int width, height;

	//Converting image path
	const char *convPath;
	convPath = path.c_str();

	FIBITMAP* image;

	try
	{
		FREE_IMAGE_FORMAT format = FreeImage_GetFileType(convPath, 0); //detection of format
		
		image = FreeImage_Load(format, convPath);
	
		image = FreeImage_ConvertTo32Bits(image);
		
		width = FreeImage_GetWidth(image);
		height = FreeImage_GetHeight(image);
	}
	catch (int e)
	{
		std::cout << "Error loading Image: " + e << std::endl;
		system("PAUSE");
		exit(-1);
	}

	BYTE *pixels = (BYTE*)FreeImage_GetBits(image);

	// if something went wrong, exit with error
	if (pixels == NULL || width == 0 || height == 0){
		std::cout << "Error loading Image " << std::endl;
		system("PAUSE");
		exit(-1);
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA/*intensity*/, width, height, 0, GL_BGRA/*luminance!!*/, GL_UNSIGNED_BYTE, pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	
}


Texture::~Texture()
{
	glDeleteTextures(1, &handle);
}


void Texture:: Bind(int unit, int textureQuality, int mipmap)
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, handle);

	if (textureQuality == 0 && mipmap == 0) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	else if (textureQuality == 0 && mipmap == 1) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	else if (textureQuality == 0 && mipmap == 2) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	else if (textureQuality == 1 && mipmap == 0) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else if (textureQuality == 1 && mipmap == 1) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else if (textureQuality == 1 && mipmap == 2) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
}