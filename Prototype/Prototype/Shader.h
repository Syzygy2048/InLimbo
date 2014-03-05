#ifndef PROTOTYPE_SHADER_HPP_
#define PROTOTYPE_SHADER_HPP_

#include <string>
#include <GL/glew.h>

namespace prototype{
	class Shader{
	public:
		Shader(const std::string& vertexShader, 
				const std::string &fragmentShader);
		~Shader();
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