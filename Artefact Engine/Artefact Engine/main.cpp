#include <iostream>

#include <GL\glew.h> //openGL extension wrangling, include before glfw and openGL because it's a bit "special".
#include <GLFW\glfw3.h> //window and periphery handling
#include <gl\GL.h> //opengl

#include "InputHandler.h"

//quick and dirty settings
#define WINDOW_TITLE "In Limbo"
#define resX 1280
#define resY 720

int main(){
	//init
	GLFWwindow* window;
	if (!glfwInit()){
		return -1;
	}

	//settings for the openGL context, additional flags: http://www.glfw.org/docs/3.0/window.html#window_hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //openGL context 3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL


	window = glfwCreateWindow(resX, resY, WINDOW_TITLE, nullptr, nullptr);

	if (!window){
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
		
	InputHandler input = InputHandler(window);
	//gameloop
	while (!glfwWindowShouldClose(window)){

		glfwSwapBuffers(window); //actually renders the frame
		glfwPollEvents();
	}

	//unloading
	glfwTerminate();

	return 0;
}