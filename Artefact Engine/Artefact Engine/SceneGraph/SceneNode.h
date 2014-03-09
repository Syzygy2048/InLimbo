#pragma once

#include <vector>
#include "../InputHandler.h"
#include <glm\glm.hpp>

class SceneNode
{
public:
	static enum NodeType
	{
		ROOT = 0x100,
		TRANSLATE = 0x101,
		MESH = 0x102,
		LIGHT = 0x103,
		CAMERA = 0x104
	};

	virtual void update(double dT, InputHandler* input);
	virtual void draw(glm::mat4 vp);

	void addNode(SceneNode* node);
	void remove(SceneNode* node);
	void cleanup();

	SceneNode(NodeType type);
	~SceneNode();

protected:
	std::vector<SceneNode*> childNodes;
private:
	const NodeType type;
	SceneNode* parent;
};

