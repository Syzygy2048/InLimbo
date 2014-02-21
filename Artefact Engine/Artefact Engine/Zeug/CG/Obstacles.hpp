#ifndef PROJECT_NAME_SCENE_ASTRONAUT_HPP_
#define PROJECT_NAME__SCENE_OBSTACLES_HPP_

#include <GL\glew.h>
#include "SceneObject.hpp"
#include"../Shader.hpp"
#include "Monster.hpp"
#include "SkyboxPart.hpp"
#include <vector>


namespace projectname{
	namespace scene{
		class Obstacles{
		public:
			Obstacles();
			virtual void initalize(std::vector<SceneObject*> &sceneGraph, Shader* shader, btDiscreteDynamicsWorld* world);
			virtual ~Obstacles();
		};
	}
}
#endif

