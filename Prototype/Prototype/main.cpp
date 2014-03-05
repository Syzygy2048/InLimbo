#include <iostream>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "Scene/Cube.h"


#define WIDTH 800
#define HEIGHT 600

using namespace prototype;
using namespace prototype::scene;


void Init();
void Update(double deltaT);
void Draw();
void Cleanup();

//variables
Shader* shader;
Cube* cube;

int main()
{
	bool running = true;
	double deltaT = 0.0f;
	double lastTime = 0.0f;

	//TODO1 ... open Window
	if(!glfwInit())
	{
		std::cout << "OOOPS" <<std::endl;
		system("PAUSE");
		exit(-1);
	}
	 //opengl setup context
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	if(!glfwOpenWindow(WIDTH, HEIGHT,0,0,0,0,0,0,GLFW_WINDOW))
	{
		glfwTerminate();
		std::cout << "OOOPS...2" <<std::endl;
		system("PAUSE");
		exit(-1);
	}

	//TODO 2 ....render loop
	//load opengl 3.3
	if(glewInit() != GLEW_OK){
		std::cout << "failed to init glew" <<std::endl;
		system("PAUSE");
		exit(-1);
	}

	//Bugfixes
	__glewGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)glfwGetProcAddress("help");
	__glewBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)glfwGetProcAddress("help");
	__glewDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)glfwGetProcAddress("help");
	 
	Init();

	//set clear color and viewport
	glClearColor(0.14f, 0.25f, 0.43f, 0.0f);
	glViewport(0,0,WIDTH, HEIGHT);

	while(running)
	{

		//clear window
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		double time = glfwGetTime();
		deltaT = time - lastTime;
		lastTime = time;

		Update(deltaT);
		Draw();

		std::cout << "frametime: " << deltaT *1000 << "ms" << std::endl;

		glfwSwapBuffers();
		running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);

		if(glGetError ()!= GL_NO_ERROR){
			std::cout << "gl error"<< std::endl;
		}
	}


	Cleanup();
	glfwTerminate();
}

void Init()
{
	glfwSetWindowTitle("Prototype");
	
	glEnable(GL_DEPTH_TEST);

	shader = new Shader("Shader/vbo_vao.vert", "Shader/vbo_vao.frag");
	cube= new Cube(glm::mat4(1.0f), shader);

	shader->useShader();
	glm::mat4 projectionMatrix = glm::translate(glm::perspective(60.0f, (float)WIDTH/(float)HEIGHT, 0.1f, 20.0f),
		glm::vec3(0.0f,0.0f,-2.0f));
	glUniformMatrix4fv(glGetUniformLocation(shader -> programHandle, "projectionMatirx"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}

void Update(double deltaT){
	cube ->update(deltaT);
}

void Draw(){
	shader -> useShader();
	glUniformMatrix4fv(
		glGetUniformLocation(shader->programHandle, "modelMatrix"),
		1, GL_FALSE, glm::value_ptr(cube->modelMatrix)
		);
	cube -> draw();
	
}

void Cleanup(){
	delete cube;
	delete shader;
}