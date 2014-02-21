#pragma once

#include "SceneNode.h"
#include <glm\glm.hpp>


class MeshNode : public SceneNode
{
public:
	MeshNode();
	~MeshNode();

	void update(double dT, InputHandler* input);
	void draw(glm::mat4* vp);
	

private:
	const std::string path = "hallo";
	
	glm::mat4 mvp;
};

