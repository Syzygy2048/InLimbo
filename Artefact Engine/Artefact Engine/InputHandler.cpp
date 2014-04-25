#include "InputHandler.h"

#include <iostream>

//#include <iostream>

InputHandler::InputHandler(GLFWwindow* w)
{
	window = w;
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//mouseWheel = 0;

	//oldMouseWheel = 0;
}

//Handles keyboard and mouse input.
void InputHandler::handleInput()
{
	pollKeyboard();
	pollMouse();
	//pollGamepads();
}

/*void InputHandler::pollGamepads()
{
	//glfwGetJoystickButtons(GLFW_JOYSTICK_1, p1Input, 10);

	glfwGetJoystickPos(GLFW_JOYSTICK_1, p1Axis, 5);
	glfwGetJoystickPos(GLFW_JOYSTICK_2, p2Axis, 5);
	//getting rid of noise in the data
	for (int i = 0; i < 5; i++)
	{
		if (abs(p1Axis[i]) < 0.15) p1Axis[i] = 0;
		if (abs(p2Axis[i]) < 0.15) p2Axis[i] = 0;
	}

} */

void InputHandler::pollKeyboard()
{
	oldSpace = space;
	w = glfwGetKey(window, GLFW_KEY_W);
	a = glfwGetKey(window, GLFW_KEY_A);
	s = glfwGetKey(window, GLFW_KEY_S);
	d = glfwGetKey(window, GLFW_KEY_D);
	space = glfwGetKey(window, GLFW_KEY_SPACE);
	esc = glfwGetKey(window, GLFW_KEY_ESCAPE);
}

void InputHandler::pollMouse()
{
	oldMousePos.x = mPosX;
	oldMousePos.y = mPosY;
	glfwGetCursorPos(window, &mPosX, &mPosY);
	//oldMouseWheel = mouseWheel;
	//mouseWheel = glfwGetMouseWheel();
	mLeftClick = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	mRightClick = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
	//mMiddleClick = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);

	
	//glfwSetCursorPos(window, 640, 360);


}

InputHandler::~InputHandler(void)
{
}
