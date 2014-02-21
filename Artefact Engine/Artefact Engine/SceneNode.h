#pragma once

#include <vector>
class SceneNode
{
public:
	SceneNode();
	~SceneNode();

	void update(double dT, );
	void draw();

	void add(SceneNode*);
	void remove(SceneNode*);

	void cleanup();

private:
	std::vector<SceneNode*> childNodes;
};

