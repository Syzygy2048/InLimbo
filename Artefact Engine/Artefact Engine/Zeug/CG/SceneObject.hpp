#ifndef PROJECT_NAME_SCENEOBJECT_HPP_
#define PROJECT_NAME_SCENE_SCENEOBJECT_HPP_

#include <glm/glm.hpp>
// würde hier hin kommen #include <btBulletDynamicsCommon.h>
// platzhalter #include <btBulletCollisionCommon.h>

#include "Animator.hpp"

#include <assimp/Importer.hpp>		// C++ importer interface
#include <assimp/scene.h>			// Output data structure
#include <assimp/postprocess.h>		// Post processing flags

namespace projectName{
	namespace scene{
		class SceneObject{
		public:
			SceneObject(glm::mat4& modelMatrix);

			virtual ~SceneObject();	//virtual destructor

		
			virtual void update(double deltaT, int key, float speed) = 0;	// =0  is an abstract Method, no implementatin in this class
			virtual void draw() = 0;
			virtual btRigidBody* getRigid() = 0;
			bool rigidExists;

			glm::mat4 modelMatrix;	//public class-variable that saves our matrix 
			glm::mat4 viewMatrix;
			glm::mat4 projectionMatrix;

			int textureID;


			//Lighting --> welche brauchen wir jetzt ?
			float diffuse[4];
			float specular[4];
			float ambient[4];
			float emission[4];
			float shininess;
		};
	}
}

#endif