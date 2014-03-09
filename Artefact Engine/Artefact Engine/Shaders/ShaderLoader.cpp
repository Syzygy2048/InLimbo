#include "ShaderLoader.h"

#include <fstream>
#include <iostream>
#include <string>


ShaderLoader::ShaderLoader()
{
}

GLuint ShaderLoader::loadShaders(const char* vertexShaderPath, const char* fragmentShaderPath, const char* geometryShaderPath)
{
	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint geometryShaderId;
	
	
	std::string vertexShaderCode;
	std::ifstream vertexShaderStream(vertexShaderPath, std::ios::in);
	if (vertexShaderStream.is_open())
	{
		std::string line = "";
		while (getline(vertexShaderStream, line))
		{
			vertexShaderCode += "\n" + line;
		}
		vertexShaderStream.close();
	}

	std::string fragmentShaderCode;

	std::ifstream fragmentShaderStream(fragmentShaderPath, std::ios::in);
	if (fragmentShaderStream.is_open())
	{
		std::string line = "";
		while (getline(fragmentShaderStream, line))
		{
			fragmentShaderCode += "\n" + line;
		}
		fragmentShaderStream.close();
	}
	
	std::string geometryShaderCode;
	if (geometryShaderPath != nullptr){
		geometryShaderId = glCreateShader(GL_GEOMETRY_SHADER);
	
		
		std::ifstream geometryShaderStream(geometryShaderPath, std::ios::in);
		if (geometryShaderStream.is_open())
		{
			std::string line = "";
			while (getline(geometryShaderStream, line))
			{
				geometryShaderCode += "\n" + line;
			}
			geometryShaderStream.close();
		}
	}

	GLint result = false;
	int infoLogLength;

	printf("Compiling shader: %s\n", vertexShaderPath);
	char const* vertexSource = vertexShaderCode.c_str();
	glShaderSource(vertexShaderId, 1, &vertexSource, NULL);
	glCompileShader(vertexShaderId);

	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertexShaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
	std::vector<char> vertexShaderErrorMessage(infoLogLength);
	glGetShaderInfoLog(vertexShaderId, infoLogLength, NULL, &vertexShaderErrorMessage[0]);
	fprintf(stdout, "%s\n", &vertexShaderErrorMessage[0]);

	
	printf("Compiling shader: %s\n", fragmentShaderPath);
	char const* fragmentSource = fragmentShaderCode.c_str();
	glShaderSource(fragmentShaderId, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShaderId);

	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragmentShaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
	std::vector<char> fragmentShaderErrorMessage(infoLogLength);
	glGetShaderInfoLog(fragmentShaderId, infoLogLength, NULL, &fragmentShaderErrorMessage[0]);
	fprintf(stdout, "%s\n", &fragmentShaderErrorMessage[0]);
	

	if (geometryShaderPath != nullptr)
	{
		printf("Compiling shader: %s\n", geometryShaderPath);
		char const* geometrySource = geometryShaderCode.c_str();
		glShaderSource(geometryShaderId, 1, &geometrySource, NULL);
		glCompileShader(geometryShaderId);

		glGetShaderiv(geometryShaderId, GL_COMPILE_STATUS, &result);
		glGetShaderiv(geometryShaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::vector<char> geometryShaderErrorMessage(infoLogLength);
		glGetShaderInfoLog(geometryShaderId, infoLogLength, NULL, &geometryShaderErrorMessage[0]);
		fprintf(stdout, "%s\n", &geometryShaderErrorMessage[0]);
	}

	GLuint shaderProgramId = glCreateProgram();
	
	glAttachShader(shaderProgramId, vertexShaderId);
	glDeleteShader(vertexShaderId);
	
	glAttachShader(shaderProgramId, fragmentShaderId);
	glDeleteShader(fragmentShaderId);

	if (geometryShaderPath != nullptr)
	{
		glAttachShader(shaderProgramId, geometryShaderId);
		glDeleteShader(geometryShaderId);
	}


	return shaderProgramId;
}


GLuint ShaderLoader::loadShaderArray(std::vector<std::string> shaderPaths)
{
	GLuint programID = glCreateProgram();

	//for error handling
	GLint result = GL_FALSE;
	int logLength;

	for (const std::string shaderPath : shaderPaths){
		GLuint shaderID;
		if (shaderPath.find("Vertex") != std::string::npos)
			shaderID = glCreateShader(GL_VERTEX_SHADER);
		else if (shaderPath.find("Fragment") != std::string::npos)
			shaderID = glCreateShader(GL_FRAGMENT_SHADER);
		else if (shaderPath.find("Geometry") != std::string::npos)
			shaderID = glCreateShader(GL_GEOMETRY_SHADER);
		//else if (shaderPath.find("TesselationControl") != std::string::npos)
			//shaderID = glCreateShader(GL_TESS_CONTROL_SHADER);
		//else if (shaderPath.find("TesselationEval") != std::string::npos)
			//shaderID = glCreateShader(GL_TESS_EVALUATION_SHADER);
		
		std::string shaderCode;
		std::ifstream codeStream(shaderPath, std::ios::in);
		if (codeStream.is_open())
		{
			std::string line = "";
			while (getline(codeStream, line))
				shaderCode += "\n" + line;
			codeStream.close();
		}

		std::cerr << "compiling shader: " << shaderPath << std::endl;
		char const * sourcePointer = shaderCode.c_str();
		glShaderSource(shaderID, 1, &sourcePointer, NULL);
		glCompileShader(shaderID);

		// Check Shader
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> shaderErrorMessage(logLength);	
		glGetShaderInfoLog(shaderID, logLength, NULL, shaderErrorMessage.data());
		fprintf(stdout, "%s\n", &shaderErrorMessage[0]);

		glAttachShader(programID, shaderID);
	}
	glLinkProgram(programID);

	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<char> programErrorMessage(logLength);
	glGetProgramInfoLog(programID, logLength, NULL, &programErrorMessage[0]);
	fprintf(stdout, "%s\n", &programErrorMessage[0]);
	return programID;
}

ShaderLoader::~ShaderLoader()
{
}
