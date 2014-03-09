#include "SceneNode.h"
#include "MeshNode.h"


SceneNode::SceneNode(NodeType t) : type(t)
{
	childNodes = std::vector<SceneNode*>();
}


SceneNode::~SceneNode()
{
}

void SceneNode::update(double dT, InputHandler* input)
{
	for (SceneNode* node : childNodes)
	{
		node->update(dT, input);
	}
}

void SceneNode::draw(glm::mat4 vp)
{
	for (SceneNode* node : childNodes)
	{
		node->draw(vp);
	}
}

void SceneNode::addNode(SceneNode* node)
{
	childNodes.push_back(node);
}