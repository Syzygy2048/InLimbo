#pragma once

#include "SceneNode.h"

class CameraNode : public SceneNode
{
public:


	void update(double dT, InputHandler* input);
	void draw();

	glm::mat4 getViewMatrix(){ return v; }

	CameraNode();
	~CameraNode();
private:
	glm::mat4 v;
};

