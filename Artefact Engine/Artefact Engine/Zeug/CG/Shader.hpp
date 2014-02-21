#ifndef PROJECCT_NAME_SHADER_HPP_
#define PROJECT_NAME_SHADER_HPP_

#include <string>
#include <GL\glew.h>

namespace projectname{
	class Shader{
	public:
		Shader(const std::string& vertexShader,			//constructor path to verteShader-file and path to framentShader-file
			const std::string& fragmentShader);
		
		~Shader();		//destructor removes shader from graphic-card when shader-class removed

		void useShader() const;

		GLuint programHandle;

	private:
		void loadShader(const std::string& shader,
			GLenum shaderType, GLuint& handle);
		void link();

		GLuint vertexHandle;
		GLuint fragmentHandle;
	};
}

#endif