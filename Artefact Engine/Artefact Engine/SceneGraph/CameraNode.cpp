#include "CameraNode.h"
#include <iostream>


CameraNode::CameraNode() : SceneNode(NodeType::CAMERA)
{
	cameraPosition = glm::vec3(3, 2, 1);
	lookAtTarget = glm::vec3(2, 2, 1);
}

void CameraNode::moveForward(float dist)
{
	glm::vec3 d(lookAtTarget.x - cameraPosition.x, 0, lookAtTarget.z - cameraPosition.z);
	cameraPosition += glm::normalize(d) * (dist/100);
	lookAtTarget += glm::normalize(d) * (dist / 100);
}
void CameraNode::moveSideward(float dist)
{
	glm::vec3 cross = glm::normalize(glm::cross(glm::vec3(0, 1, 0), lookAtTarget - cameraPosition));
	cameraPosition += cross * (dist / 100);
	lookAtTarget += cross * (dist / 100);
}


void CameraNode::rotate(float rotateX, float rotateY)
{
	
	glm::vec3 delta = lookAtTarget - cameraPosition;
	
	float r = sqrt(delta.x * delta.x + delta.z * delta.z+ delta.y * delta.y);

	
	float alpha = atan2(delta.z, delta.y); 
	float beta = atan2(delta.z, delta.x);
	
	alpha += rotateY/300;
	beta += rotateX/300;
	
	if (rotateY > 0)
	{
		lookAtTarget.y = cameraPosition.y + r * cos(alpha);
		lookAtTarget.z = cameraPosition.z + r * sin(alpha);
	}
	if (rotateX > 0)
	{
		lookAtTarget.x = cameraPosition.x + r * cos(beta);
		lookAtTarget.z = cameraPosition.z + r * sin(beta);
	}
	
	//lookAtTarget.z = cameraPosition.z + r * sin(alpha);
	
	/*
	float r2 = sqrt(delta.y * delta.y + delta.z*delta.z);
	float beta = atan2(delta.y, delta.z);
	beta += rotateY / 300;

	lookAtTarget.y = cameraPosition.y + r2 * cos(beta);
	lookAtTarget.z = cameraPosition.z + r2 * sin(beta);
	*/


}
 
glm::mat4 CameraNode::getViewMatrix()
{
	glm::mat4 view = glm::lookAt(
		cameraPosition,
		lookAtTarget,
		glm::vec3(0.f, 1.f, 0.f)
		);
	return view;
}
CameraNode::~CameraNode()
{
}
