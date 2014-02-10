#include <iostream>

#include <GL\glew.h> //openGL extension wrangling, include before glfw and openGL because it's a bit "special".
#include <GLFW\glfw3.h> //window and periphery handling
#include <gl\GL.h> //opengl

#include "InputHandler.h"
#include "MazeTile.h"

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

	glewInit();
		
	InputHandler input = InputHandler(window);
	
	GLuint vertexArrayID;
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);
	
	static const GLfloat vertexData[] = { -1.0f, -1.0f, 0.0f,
											1.0f, -1.0f, 0.0f,
											0.0f, 1.0f, 0.0f
										};

	GLuint vertexBufferID;
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	
	MazeTile startTile;
	//gameloop
	while (!glfwWindowShouldClose(window)){


		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
			);

		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		glDisableVertexAttribArray(0);


		glfwSwapBuffers(window); //actually renders the frame
		glfwPollEvents();
	}

	//unloading
	glfwTerminate();

	return 0;
}