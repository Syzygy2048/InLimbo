#pragma once

#include <GL\glfw3.h>
#include <glm\glm.hpp>

class InputHandler
{
public:

	//unsigned char p1Input[10];
	//float p1Axis[5];
	
	bool w;
	bool a;
	bool s;
	bool d;

	bool space;
	bool oldSpace;
	
	double mPosX;
	double mPosY;
	//int mouseWheel;
	bool mLeftClick;
	bool mRightClick;
	//bool mMiddleClick;
	bool esc;
	
	glm::vec2 oldMousePos;
	//int oldMouseWheel;

	void handleInput();

	InputHandler(GLFWwindow* w);
	~InputHandler();

private:
	GLFWwindow* window;

	void pollKeyboard();
	void pollMouse();

	
	//void pollGamepads();
};
