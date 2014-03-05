#ifndef PROJECT_NAME_SCENE_ASTRONAUT_HPP_
#define PROJECT_NAME_SCENE_ALTIMETER_HPP_

#include <GL\glew.h>
#include <GL\glfw.h>
#include "SceneObject.hpp"
#include"../Shader.hpp"
#endif

namespace projectname{
	namespace scene{
		class Altimeter: public SceneObject{		
		public:
			Altimeter(glm::mat4& matrix, Shader* shader);
			virtual ~Altimeter();

			virtual void update(double deltaT, int key, float speed);
			virtual void draw();
			virtual btRigidBody* getRigid();
			bool rigidExists;

		private:
			GLuint vao;
			GLuint positionBuffer, normalBuffer, faceBuffer, uvBuffer;
			Shader* shader;
			
			//Face-Count
			int numFaces;
		};
	}
}