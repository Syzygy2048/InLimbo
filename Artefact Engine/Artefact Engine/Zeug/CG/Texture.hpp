#ifndef PROJECT_NAME_TEXTURE_HPP_
#define PROJECT_NAME_TEXTURE_HPP_

#include <string>
#include <GL/glew.h>

namespace projectname{
	class Texture{

	public:
		Texture(const std::string& path);
		~Texture();

		void Bind(int unit, int textureQuality, int mipmap);

	private:
		GLuint handle;
	};
}

#endif