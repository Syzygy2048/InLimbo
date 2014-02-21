#ifndef PROJECT_NAME_SCENE_MONSTER_HPP_
#define PROJECT_NAME_SCENE_CAMERA_HPP_

#include <GL\glew.h>
#include <GL\glfw.h>
#include "SceneObject.hpp"
#include "../Shader.hpp"

#endif


namespace projectName{
	namespace scene{
		class Camera: public SceneObject{		//Camera extends SceneObject
		public:
			Camera(glm::mat4& matrix, Shader* shader);
			
			virtual void update(double deltaT, int key, float speed);
			virtual void draw();

			virtual btRigidBody* getRigid();
			bool rigidExists;

			//Get Angles
			virtual float getHorizontalAngle();
			virtual float getVerticalAngle();

		private:
			Shader* shader;
		};
	}
}

