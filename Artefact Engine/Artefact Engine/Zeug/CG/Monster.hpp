#ifndef PROJECT_NAME_SCENE_CUBE_HPP_
#define PROJECT_NAME_SCENE_ASTRONAUT_HPP_

#include <GL\glew.h>
#include "SceneObject.hpp"
#include"../Shader.hpp"
// hier sollte bullet reinkommen #include <btBulletDynamicsCommon.h>
//bullet einfuegen#include <btBulletCollisionCommon.h>
#endif

struct BoneStructure {
	glm::vec4* bsWeights;
	glm::vec4* bsBoneIndices;

	BoneStructure(){
		bsWeights = NULL;
		bsBoneIndices = NULL;
	}

	~BoneStructure(){
		delete[] bsBoneIndices;
		bsBoneIndices = NULL;

		delete[] bsWeights;
		bsWeights = NULL;
	}
};

namespace projectname{
	namespace scene{
		class Monster: public SceneObject{		//Monster extends SceneObject
		public:
			Monster(glm::mat4& matrix, Shader* shader);
			virtual ~Monster();

			virtual void update(double deltaT, int key, float speed);
			virtual void draw();
			virtual void RenderNode(aiNode * pNode);	
			//virtual void initializeBullet(float x, float y, float z, btDiscreteDynamicsWorld* world);
			virtual btRigidBody* getRigid();
			
			static btConvexHullShape* monsterCollShape;
			static bool setMonsterColl;
			static btScalar monstermass;
			static btVector3 monsterinertia;
			btRigidBody* monsterrigid;
			bool rigidExists;
			

			//Animator* animator;


		private:
			GLuint vao;
			GLuint positionBuffer, normalBuffer, faceBuffer, uvBuffer, weightBuffer, indexBuffer;
			Shader* shader;
			
			//Face-Count
			int numFaces;
			glm::mat4 initMatrix;

			//Lightning
			GLuint diffuseBuffer ,ambientBuffer ,specularBuffer, emissionBuffer, shininessBuffer;

			//Animation
			BoneStructure* bStruct;
			const aiScene* aScene;
			const struct aiMesh* mesh;
		};
	}
}

