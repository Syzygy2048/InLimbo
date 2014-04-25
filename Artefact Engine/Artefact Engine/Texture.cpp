#include "Texture.h"

#include <iostream>

Texture::Texture(const std::string& path)
{
	FreeImage_Initialise(TRUE);
	
	glGenTextures(1, &texture);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);	// type of texture 

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR/*_MIPMAP_LINEAR*/);
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

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8/*intensity*/, width, height, 0, GL_BGRA/*luminance!!*/, GL_UNSIGNED_BYTE, pixels);
	//glGenerateMipmap(GL_TEXTURE_2D);
	
}


Texture::~Texture()
{
	glDeleteTextures(1, &texture);
}


void Texture:: Bind(int unit, int textureQuality, int mipmap)	//not sure how this is supposed to be used, pretty sure the texture should only be bound once and not every time the level of detail changes, although I might be mistaken, seems like relatively slow changes happen in games all the time (texture popin)
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, texture);

	if (textureQuality == 0 && mipmap == 0) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	else if (textureQuality == 0 && mipmap == 1) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST/*_MIPMAP_NEAREST*/);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	else if (textureQuality == 0 && mipmap == 2) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST/*_MIPMAP_LINEAR*/);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	else if (textureQuality == 1 && mipmap == 0) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else if (textureQuality == 1 && mipmap == 1) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR/*_MIPMAP_NEAREST*/);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else if (textureQuality == 1 && mipmap == 2) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR/*_MIPMAP_LINEAR*/);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
}