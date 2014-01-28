#include <iostream>

#include <GL\glew.h>
#include <GLFW\glfw3.h> //window and periphery handling
#include <gl\GL.h> //opengl

//quick and dirty settings
#define WINDOW_TITLE "In Limbo"
#define resX 1280
#define resY 720

int main(){
	//init
	bool isRunning = true;

	GLFWwindow* window;
	if (!glfwInit()){
		return -1;
	}
	window = glfwCreateWindow(resX, resY, WINDOW_TITLE, nullptr, nullptr);

	if (!window){
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
		

	//gameloop
	while (!glfwWindowShouldClose(window)){

		glfwSwapBuffers(window); //actually renders the frame
		glfwPollEvents();
	}

	//unloading
	glfwTerminate();

	return 0;
}