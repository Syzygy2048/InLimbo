#pragma once

#include <GL/glew.h>
#include <string>

class Shader
{
public:
	static enum ShaderType{
		FRAGMENT = 0x001,
		VERTEX = 0x002,
		GEOMETRY = 0x003
	};
	

	Shader(ShaderType type, std::string path);
	~Shader();


	void loadShader();
	
	void setShaderId(GLuint id);
	GLuint getShaderId();
	
	

private:
	const std::string path;
	const ShaderType type;

	GLuint shaderId;
};

