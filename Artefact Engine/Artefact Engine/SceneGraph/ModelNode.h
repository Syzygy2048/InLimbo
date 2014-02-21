#pragma once

#include "SceneNode.h"
#include <glm\glm.hpp>
class ModelNode : SceneNode
{
public:
	ModelNode();
	~ModelNode();

	void update(double dT, glm::mat4* vp);
	void draw();
	

private:
	glm::mat4 mvp;

};

