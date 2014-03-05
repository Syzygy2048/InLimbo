#include "Cube.h"
#include <glm/gtc/matrix_transform.hpp>


using namespace prototype;
using namespace prototype::scene;

Cube::Cube(glm::mat4& matrix, Shader* _shader):SceneObject(matrix), shader(_shader){
	//load data to buffer
	
	glGenBuffers(1,&positionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER,positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, CUBE_VERTEX_COUNT*3*sizeof(float), positions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER,0);

	glGenBuffers(1,&normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER,normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, CUBE_VERTEX_COUNT*3*sizeof(float), normals, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER,0);

	glGenBuffers(1,&indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, CUBE_INDEX_COUNT*3*sizeof(unsigned int), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

	//generate bindings
	glGenVertexArrays(1, &vao);
	//cout << "OOOPS" << std::endl;
	glBindVertexArray(vao);
	// cout << "OOOPS" << std::endl;
	
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	GLint positionIndex=glGetAttribLocation(shader ->programHandle, "position");
	glEnableVertexAttribArray(positionIndex);
	glVertexAttribPointer(positionIndex, 3, GL_FLOAT,GL_FALSE,0,0);

	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	GLint normalIndex=glGetAttribLocation(shader ->programHandle, "normal");
	glEnableVertexAttribArray(normalIndex);
	glVertexAttribPointer(normalIndex, 3, GL_FLOAT,GL_FALSE,0,0);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Cube::~Cube(){
	glDeleteBuffers(1,&positionBuffer);
	glDeleteBuffers(1,&indexBuffer);
	glDeleteVertexArrays(1,&vao);

}

void Cube::update(double deltaT){
	modelMatrix = glm::rotate(modelMatrix, 90.0f*(float)deltaT, glm::vec3(0.0f, 1.0f, 0.0f));

}

void Cube::draw(){
		glBindVertexArray(vao);

		glDrawElements(GL_TRIANGLES,CUBE_INDEX_COUNT,GL_UNSIGNED_INT,0);

		glBindVertexArray(0);

}

const float Cube::positions[CUBE_VERTEX_COUNT*3] ={
	//Back
	-0.5f, -0.5f, -0.5f, 
	-0.5f, 0.5f, -0.5f,
	0.5f, 0.5f, -0.5f, 
	0.5f, -0.5f, -0.5f, 

	//Front
	-0.5f, -0.5f, 0.5f, 
	0.5f, -0.5f, 0.5f,
	0.5f, 0.5f, 0.5f, 
	-0.5f, 0.5f, 0.5f, 

	//top
	0.5f, 0.5f, 0.5f, 
	0.5f, 0.5f, -0.5f,
	-0.5f, 0.5f, -0.5f, 
	-0.5f, 0.5f, 0.5f, 
	
	//bottom
	0.5f, -0.5f, 0.5f, 
	-0.5f, -0.5f, 0.5f,
	-0.5f, -0.5f, -0.5f, 
	0.5f, -0.5f, -0.5f, 

	//right
	0.5f, 0.5f, 0.5f, 
	0.5f, -0.5f, 0.5f,
	0.5f, -0.5f, -0.5f, 
	0.5f, 0.5f, -0.5f, 

	//left
	-0.5f, 0.5f, 0.5f, 
	-0.5f, 0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f, 
	-0.5f, -0.5f, 0.5f, 
};

const unsigned int Cube::indices[CUBE_INDEX_COUNT]={
	0,1,2,
	0,2,3,

	4,5,6,
	4,6,7,

	8,9,10,
	8,10,11,

	12,13,14,
	12,14,15,

	16,17,18,
	16,18,19,

	20,21,22,
	20,22,23,

};

const float Cube::normals[CUBE_VERTEX_COUNT*3] = {
0.0f, 0.0f, -1.0f,
0.0f, 0.0f, -1.0f,
0.0f, 0.0f, -1.0f,
0.0f, 0.0f, -1.0f,
	
0.0f, 0.0f, 1.0f,
0.0f, 0.0f, 1.0f,
0.0f, 0.0f, 1.0f,
0.0f, 0.0f, 1.0f,

0.0f, 1.0f, 0.0f,
0.0f, 1.0f, 0.0f,
0.0f, 1.0f, 0.0f,
0.0f, 1.0f, 0.0f,

0.0f, -1.0f, 0.0f,
0.0f, -1.0f, 0.0f,
0.0f, -1.0f, 0.0f,
0.0f, -1.0f, 0.0f,

1.0f, 0.0f, 0.0f,
1.0f, 0.0f, 0.0f,
1.0f, 0.0f, 0.0f,
1.0f, 0.0f, 0.0f,

-1.0f, 0.0f, 0.0f,
-1.0f, 0.0f, 0.0f,
-1.0f, 0.0f, 0.0f,
-1.0f, 0.0f, 0.0f,


};


