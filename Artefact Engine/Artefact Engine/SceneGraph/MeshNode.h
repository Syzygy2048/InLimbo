#pragma once

#include "SceneNode.h"
#include <glm\glm.hpp>


class MeshNode : public SceneNode
{
public:
	MeshNode(std::string path);
	~MeshNode();

	void update(double dT, InputHandler* input);
	void draw(glm::mat4* vp);
	
	void setVao(GLuint v){ vao = v; }
	void setUvVbo(GLuint vbo){ uvVBO = vbo; }
	void setPosVbo(GLuint vbo){ posVBO = vbo; }
	void setNormVbo(GLuint vbo){ normVBO = vbo; }
	void setWeightVbo(GLuint vbo){ weightVBO = vbo; }
	void setIndexVbo(GLuint vbo){ indexVBO = vbo; }

private:
	const std::string path;
	
	GLuint vao;
	GLuint uvVBO, posVBO, normVBO, weightVBO, indexVBO;
	glm::mat4 mvp;
};

