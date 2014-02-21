#ifndef PROJECT_NAME_SCENE_ASTRONAUT_HPP_
#define PROJECT_NAME_SCENE_SKYBOXPART_HPP_

#include <GL\glew.h>
#include "SceneObject.hpp"
#include"../Shader.hpp"

#endif


namespace projectname{
	namespace scene{
		class SkyboxPart: public SceneObject{		
		public:
			SkyboxPart(glm::mat4& matrix, Shader* shader);
			virtual ~SkyboxPart();

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

