#include "Camera.hpp"
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

using namespace projectname;
using namespace projectname::scene;

glm::vec3 position;
glm::vec3 direction;
glm::vec3 up;
glm::vec3 right;

float pi = 3.141592653589793238462f;
float horAngle;
float verAngle;
float initFoV;
float foV;

int mouse;
int lastMouse;

const float mouseSpeed = 0.005f;

Camera::Camera(glm::mat4& matrix, Shader* _shader) : SceneObject(matrix), shader(_shader){
	
	rigidExists = false;
	//Initial position - pos. z-achse
	position = glm::vec3(0.0f, 0.2f, 1.2f);
	//position = glm::vec3(0.0f, 0.0f, 0.0f);
	//Initial horziontal angle - richtung neg. z-achse
	horAngle = pi;
	//Initial vertical angle
	verAngle = 0.0f;
	//Initial Field of View
	initFoV = 25.0f;

	foV = 0.0f;
	mouse = 0;
	lastMouse = 0;
}

void Camera::update(double deltaT, int key, float speed){

	//Get mouse position
	int xpos, ypos;
	glfwGetMousePos(&xpos, &ypos);

	int width, height;
	glfwGetWindowSize(&width, &height);

	lastMouse = mouse;
	mouse = glfwGetMouseWheel();

	if(key == 7) {
		horAngle = pi;
		verAngle = 0.0f;
		initFoV = 25.0f;
		glfwSetMouseWheel(0);
		mouse = 0;
	}
	else {
		//Reset mouse position for next frame
		glfwSetMousePos(width/2, height/2);

		//Calculate new orientation
		horAngle += mouseSpeed * float(width/2 - xpos);
		verAngle += mouseSpeed * float(height/2 - ypos);
	}

	foV = initFoV - 5 * mouse;
	if(foV < 5.0f) foV = 5.0f;
	if(foV > 100.0f) foV = 100.0f;

	//Direction: Spherical coordinates to CAr66tesion coordinates
	glm::vec3 direction(cos(verAngle) * sin(horAngle), sin(verAngle), cos(verAngle) * cos(horAngle));

	//Right vector
	right = glm::vec3(sin(horAngle - pi/2.0f), 0, cos(horAngle - pi/2.0));

	//Up vector
	up = glm::cross(right, direction);

	//Position der Kamera, wo die Kamera hinschaut, wo "oben ist"
	viewMatrix = glm::lookAt(position, position + direction, up);

	projectionMatrix = glm::perspective(foV, (float)width / (float) height, 0.1f, 200000000.0f);
}

void Camera::draw(){}

float Camera::getHorizontalAngle(){
	return horAngle;
}

float Camera::getVerticalAngle(){
	return verAngle;
}

btRigidBody* Camera::getRigid() {
	
	return nullptr;
}