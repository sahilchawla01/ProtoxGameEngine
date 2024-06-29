#pragma once

#define GLFW_INCLUDE_NONE

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

class ACamera;

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
	static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
	static void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
	double GetTimeElapsedSinceLaunch();
	void SetTimeElapsedSinceLaunch(int TimeElapsedSinceLaunch);
	void ProcessInput(GLFWwindow* window);

	void InitialiseGame();

	//Game global variables functions
	void SetDeltaTime(float DeltaTime);
	float GetDeltaTime() { return deltaTime; };
	void SetLastFrameTime(float LastFrameTime);
	float GetLastFrameTime() { return lastFrame; };

	//Simply gets projection matrix of active camera
	glm::mat4 GetProjectionMatrix();
	//Simply gets view matrix of active camera
	glm::mat4 GetViewMatrix();

	//Gives a pointer to the currently active camera
	ACamera* GetActiveCamera();


public: 
	bool bShouldGameRun = true;


	//GLFW related variables
	int windowWidth;
	int windowHeight;

private:
	
	double TimeElapsed = 0.f;

private:
	// -- Global variables --
	//Time elapsed between last and current frame
	float deltaTime = 0.f; 
	//Time elapsed until last frame
	float lastFrame = 0.f;

	//--All World's Actor variables -- 
	std::unique_ptr<ACamera> activeCamera = nullptr;

	GLFWwindow** windowPointer = nullptr;

};