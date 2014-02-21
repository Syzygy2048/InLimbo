#include "Obstacles.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

using namespace projectname;
using namespace projectname::scene;


Monster* monster;


//Organizing our Scene

Monster* obstacleMonster1;

//Create Skybox
SkyboxPart* Sky_top;
SkyboxPart* Sky_bottom;
SkyboxPart* Sky_left;
SkyboxPart* Sky_right;
SkyboxPart* Sky_front;
SkyboxPart* Sky_back;

Obstacles::Obstacles() {} //Konstruktor

Obstacles::~Obstacles() {} //Dekonstruktor

void Obstacles::initalize(std::vector<SceneObject*> &sceneGraph, Shader* shader, btDiscreteDynamicsWorld* world) {
	
	
	monster = new Monster(glm::scale(glm::mat4(1.0f), glm::vec3(0.002f)), shader);
	
	sceneGraph.push_back(monster);		

	//Create SkyBox
	Sky_top = new SkyboxPart(glm::rotate(glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(10000.0f)), glm::vec3(0.0f,10.0f,0.0f)),180.0f,glm::vec3(0.0f,0.0f,1.0f)), shader);
	sceneGraph.push_back(Sky_top);

	Sky_bottom = new SkyboxPart(glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(10000.0f)), glm::vec3(0.0f,-10.0f,0.0f)), shader);
	sceneGraph.push_back(Sky_bottom);

	Sky_left = new SkyboxPart(glm::rotate(glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(10000.0f)), glm::vec3(10.0f,0.0f,0.0f)),90.0f,glm::vec3(0.0f,0.0f,1.0f)), shader);
	sceneGraph.push_back(Sky_left);

	Sky_right = new SkyboxPart(glm::rotate(glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(10000.0f)), glm::vec3(-10.0f,0.0f,0.0f)),270.0f,glm::vec3(0.0f,0.0f,1.0f)), shader);
	sceneGraph.push_back(Sky_right);

	Sky_back = new SkyboxPart(glm::rotate(glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(10000.0f)), glm::vec3(0.0f,0.0f,10.0f)),270.0f,glm::vec3(1.0f,0.0f,0.0f)), shader);
	sceneGraph.push_back(Sky_back);

	Sky_front = new SkyboxPart(glm::rotate(glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(10000.0f)), glm::vec3(0.0f,0.0f,-10.0f)),90.0f,glm::vec3(1.0f,0.0f,0.0f)), shader);
	sceneGraph.push_back(Sky_front);
	
	


