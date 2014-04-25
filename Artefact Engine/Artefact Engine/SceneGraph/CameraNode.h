#pragma once

#include "SceneNode.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

class CameraNode : public SceneNode
{
public:
	CameraNode();
	~CameraNode();

	void moveForward(float dist);
	void moveSideward(float dist);
	void rotate(float rotateX, float rotateY);

	//void set
	glm::mat4 getViewMatrix();
private:
	glm::vec3 cameraPosition;
	glm::vec3 lookAtTarget;
};

