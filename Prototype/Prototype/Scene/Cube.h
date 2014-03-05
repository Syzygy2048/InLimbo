#ifndef PROTOTYPE_SCENE_CUBE_HPP_
#define PROTOTYPE_SCENE_CUBE_HPP_

#include <GL/glew.h>
#include <GL\GLU.h>
#include "SceneObject.h"
#include "../Shader.h"


#define CUBE_VERTEX_COUNT 24
#define CUBE_INDEX_COUNT 36


namespace prototype{

	namespace scene{
	
		class Cube : public SceneObject{
		public:
			Cube(glm::mat4& matrix, Shader* _shader);
			virtual ~Cube();

			virtual void update(double deltaT);
			virtual void draw();

		private:
			GLuint vao;

			GLuint positionBuffer, indexBuffer, normalBuffer;
			Shader* shader;

			static const float positions[CUBE_VERTEX_COUNT * 3];
			static const float normals[CUBE_VERTEX_COUNT*3];
			static const unsigned int indices[CUBE_INDEX_COUNT];
		};
	
	}


}

#endif
