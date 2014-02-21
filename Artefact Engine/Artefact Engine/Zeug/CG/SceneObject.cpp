#include "SceneObject.hpp"

using namespace NameProject::scene;

SceneObject::SceneObject(glm::mat4& _modelMatrix) : modelMatrix(_modelMatrix){}					//Class-name::method-name

SceneObject::~SceneObject(){}	//implement destructor