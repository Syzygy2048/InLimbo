#include "Shader.hpp"

#include <iostream>
#include <fstream>

using namespace projectname;
using namespace std;


Shader::Shader(const string& vertexShader, const string& fragmentShader) 
	: programHandle(0), vertexHandle(0), fragmentHandle(0) {
	
		programHandle = glCreateProgram();

		if(programHandle == 0){
			std::cout << "could not retrieve program handle!" << std::endl;
			system("Pause");
			exit(-1);
		}

		//load shader
		loadShader(vertexShader, GL_VERTEX_SHADER, vertexHandle);
		loadShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentHandle);

		//link program
		link();
}

Shader::~Shader(){
	glDeleteProgram(programHandle);
	glDeleteShader(vertexHandle);
	glDeleteShader(fragmentHandle);
}

void Shader::useShader() const{
	glUseProgram(programHandle);
}

void Shader::loadShader(const std::string& shader, GLenum shaderType, GLuint& handle) {
	//load Shader from text-file

	std::ifstream shaderFile(shader);	//input-filestream to our shader-file

	if(shaderFile.good()){
		std::string code = std::string(
			std::istreambuf_iterator<char>(shaderFile),
			std::istreambuf_iterator<char>());

		shaderFile.close();

		handle = glCreateShader(shaderType);	//vertex or fragment Shader

		if(handle == 0){
			std::cout << "failed to create shader" << std::endl;
			system("PAUSE");
			exit(-1);
		}

		const char* codePtr = code.c_str();
		glShaderSource(handle, 1, &codePtr, NULL);
		glCompileShader(handle);	

		GLint succeded;

		glGetShaderiv(handle, GL_COMPILE_STATUS, &succeded);

		if(succeded == GL_FALSE || !glIsShader(handle)){
		
			//log auslesen und ausgeben
			GLint logSize;
			glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &logSize);
			GLchar* message = new char[logSize];

			glGetShaderInfoLog(handle, logSize, NULL, message);
			std::cout << "failed to compile shader" << std::endl;
			std::cout << message << std::endl;
			system("PAUSE");
			delete[] message;
			exit(-1);
		}
	}else{
		std::cout << "failed to open shader file" << std::endl;
		system("PAUSE");
		exit(-1);
	}

}

void Shader::link(){

	//Attach shader to program
	glAttachShader(programHandle, vertexHandle);
	glAttachShader(programHandle, fragmentHandle);

	//Bind output
	glBindFragDataLocation(programHandle, 0, "fragColor");		//variable that writes color to backbuffer

	//Linken 
	glLinkProgram(programHandle);

	//Fehler ueberpruefen
	GLint succeded;
	glGetProgramiv(programHandle, GL_LINK_STATUS, &succeded);

	if(!succeded){

		//Fehler ausgeben
		GLint logSize;
		glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &logSize);

		GLchar* message = new char[logSize];
		glGetProgramInfoLog(programHandle, logSize, NULL, message);

		std::cout << "failed to link shader program" << std::endl;
		std::cout << message << std::endl;
		system("Pause");

		delete[]message;
		exit(-1);	
	}
}