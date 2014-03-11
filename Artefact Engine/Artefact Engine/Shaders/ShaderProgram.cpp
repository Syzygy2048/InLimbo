#include "ShaderProgram.h"


ShaderProgram::ShaderProgram()
{
	programId = -1;
	
}


void ShaderProgram::buildProgram()
{
	programId = glCreateProgram();

	glAttachShader(programId, vertexShader->getShaderId());
	glAttachShader(programId, fragmentShader->getShaderId());
	if (geometryShader != nullptr) glAttachShader(programId, geometryShader->getShaderId());

	glLinkProgram(programId);
}

void ShaderProgram::setVertexShader(Shader* shader)
{
	vertexShader = shader;
}

void ShaderProgram::setFragmentShader(Shader* shader)
{
	fragmentShader = shader;
}
void ShaderProgram::setGeometryShader(Shader* shader)
{
	geometryShader = shader;
}

GLuint ShaderProgram::getShaderId()
{
	if (programId == -1) buildProgram();
	return programId;
}

ShaderProgram::~ShaderProgram()
{
}
