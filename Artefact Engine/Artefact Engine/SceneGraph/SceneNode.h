#pragma once

#include <vector>
#include "../InputHandler.h"
#include <glm\glm.hpp>
#include "../Enums/SceneGraphEnums.h"

class SceneNode
{
public:
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

