#ifndef PROTOTYPE_SCENE_SCENEOBJECT_HPP_
#define PROTOTYPE_SCENE_SCENEOBJECT_HPP_

#include <glm/glm.hpp>

namespace prototype{
	namespace scene{
		class SceneObject{
		public:
			SceneObject(glm::mat4& modelMatrix);
			virtual ~SceneObject();
			virtual void update(double deltaT)= 0;
			virtual void draw() = 0;

			glm::mat4 modelMatrix;

		};
	}

}


#endif