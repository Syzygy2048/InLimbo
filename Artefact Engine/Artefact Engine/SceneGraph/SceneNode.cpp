#include "SceneNode.h"


SceneNode::SceneNode()
{
	childNodes = std::vector<SceneNode*>();
}


SceneNode::~SceneNode()
{
}

void SceneNode::update()
{
	for (SceneNode* node : childNodes)
	{
		node->update();
	}
}

void SceneNode::draw()
{
	for (SceneNode* node : childNodes)
	{
		node->draw();
	}
}
