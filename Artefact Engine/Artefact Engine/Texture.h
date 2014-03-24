#pragma once

#include <string>
#include <GL\glew.h>


class Texture
{
public:
	Texture(const std::string& path);
	~Texture();

	void Bind(int unit, int textureQuality, int mipmap);

private:
	GLuint handle;
	
};

