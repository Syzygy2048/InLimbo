#pragma once

#include <GL\glew.h>

#include "Shader.h"

class ShaderProgram
{
public:
	
	void setVertexShader(Shader* shader);
	void setFragmentShader(Shader* shader);
	void setGeometryShader(Shader* shader);

	GLuint getShaderId();
	void buildProgram();

	ShaderProgram();
	~ShaderProgram();



protected: 
	Shader* vertexShader;
	Shader* fragmentShader;
	Shader* geometryShader;
	

private:
	GLuint programId;
	
	
	
};

