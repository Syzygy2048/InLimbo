#pragma once

#include <GL\glew.h>
#include <vector>

class ShaderLoader
{
public:
	GLuint loadShaders(const char* vertexShaderPath, const char* fragmentShaderPath, const char* geometryShaderPath);
	GLuint loadShaderArray(std::vector<std::string> shaderPaths);
	
	ShaderLoader();
	~ShaderLoader();
};

