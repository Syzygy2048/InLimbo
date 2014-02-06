#pragma once

#include <GLFW\glfw3.h>

class InputHandler
{
public:
	InputHandler(GLFWwindow* win);
	~InputHandler();

	void registerCallback(int key, void* function);
	void unregisterCallback(int key);

private:
	GLFWwindow* window;

};

