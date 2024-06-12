#pragma once

#define GLFW_INCLUDE_NONE

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
/*
* GLFW is used to handle input, create a window, etc.
* GLAD retrieves location of functions that implement OpenGL specifications and stores them. These functions are implemented as per driver-basis and is used to retrieve the function implementation at run-time.
*/
class Game
{

public:
	Game(GLFWwindow** windowPointer);

	static void InputKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
	double GetTimeElapsedSinceLaunch();
	void SetTimeElapsedSinceLaunch(int TimeElapsedSinceLaunch);

	void InitialiseGame();
	void SetViewMatrix(glm::highp_mat4 newViewMatrix);
	glm::mat4 GetViewMatrix();

public: 
	bool bShouldGameRun = true;


	//GLFW related variables
	int windowWidth;
	int windowHeight;

private:
	
	double TimeElapsed = 0.f;

private:

	//Camera variables and functions
	glm::mat4 viewMatrix = glm::mat4(1.f);
	glm::vec3 currentCameraPosition = glm::vec3(0.f);
	glm::vec3 startCameraPosition = glm::vec3(0.f, 0.f, -10.f);
	float cameraStep = 0.5f;
	float cameraSpeed = 1.f;

	void TranslateViewMatrix(glm::vec3 translateVector);

	GLFWwindow** windowPointer = nullptr;

};