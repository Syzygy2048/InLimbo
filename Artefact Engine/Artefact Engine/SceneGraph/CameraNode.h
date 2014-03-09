#pragma once

#include "SceneNode.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

class CameraNode : public SceneNode
{
public:
	CameraNode();
	~CameraNode();


	glm::mat4 getViewMatrix(){ return viewMatrix; }

private:
	glm::mat4 viewMatrix;
};

