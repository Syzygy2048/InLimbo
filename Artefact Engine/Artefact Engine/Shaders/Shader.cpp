#include "Shader.h"
#include "ShaderLoader.h"


Shader::Shader(ShaderType type, std::string path) : type(type), path(path)
{
	shaderId = -1;
}

void Shader::loadShader()
{
	ShaderLoader::getInstance()->loadShader(path, type);
}

void Shader::setShaderId(GLuint id)
{
	shaderId = id;
}
GLuint Shader::getShaderId()
{
	if (shaderId == -1) loadShader();
	return shaderId;
}

Shader::~Shader()
{
}
