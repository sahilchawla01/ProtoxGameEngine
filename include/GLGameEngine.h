#pragma once

#define GLFW_INCLUDE_NONE

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

/*
* GLFW is used to handle input, create a window, etc.
* GLAD retrieves location of functions that implement OpenGL specifications and stores them. These functions are implemented as per driver-basis and is used to retrieve the function implementation at run-time.
*/
class Game
{

public:
	Game();

	static void InputKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);

public: 
	bool bShouldGameRun = true;

};