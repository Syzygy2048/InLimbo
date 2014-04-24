#pragma once

#include <GL\glew.h>
#include <vector>
#include <string>
#include <map>

#include "Shader.h"
#include "ShaderProgram.h"

class ShaderLoader
{
public:

	//GLuint loadShaderArray(std::vector<std::string> shaderPaths);
	
	Shader* loadShader(std::string, Shader::ShaderType type);
	ShaderProgram* getShaderProgram(std::string shaderProgramIdentifier);

	static ShaderLoader* getInstance();
	~ShaderLoader();

private:
	ShaderLoader();
	ShaderLoader(ShaderLoader const&);
	void operator=(ShaderLoader const&);
	
	std::map<std::string, Shader*> shaders;
	std::map<std::string, ShaderProgram*> shaderPrograms;

	
};

