#include "CameraNode.h"


CameraNode::CameraNode() : SceneNode(NodeType::CAMERA)
{
	viewMatrix = glm::lookAt(
		glm::vec3(3, 4, 3),		// the position of your camera, in world space
		glm::vec3(0, 0, 0),		// where you want to look at, in world space
		glm::vec3(0, 1, 0)     // up vector
		);
}

bool CameraNode::isActiveCamera()
{
	return isActive;
}

CameraNode::~CameraNode()
{
}
