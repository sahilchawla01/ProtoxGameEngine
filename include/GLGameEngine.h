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
	static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
	static void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
	double GetTimeElapsedSinceLaunch();
	void SetTimeElapsedSinceLaunch(int TimeElapsedSinceLaunch);
	void ProcessInput(GLFWwindow* window);

	void InitialiseGame();

	//-- MVP matrix functions --
	void SetViewMatrix(glm::highp_mat4 newViewMatrix);
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix() const { return projectionMatrix; };

	//Game global variables functions
	void SetDeltaTime(float DeltaTime);
	float GetDeltaTime() { return deltaTime; };
	void SetLastFrameTime(float LastFrameTime);
	float GetLastFrameTime() { return lastFrame; };


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
	glm::mat4 projectionMatrix = glm::mat4(1.f);
	glm::mat4 viewMatrix = glm::mat4(1.f);

	//-- MVP related functions --
	//Update the current view matrix to its new values
	void UpdateViewMatrix();
	void UpdateProjectionMatrix();

	//Camera variables and functions
	glm::vec3 currentCameraPosition = glm::vec3(0.f);
	glm::vec3 startCameraPosition = glm::vec3(0.f, 0.f, 10.f);
	glm::vec3 cameraFront = glm::vec3(0.f, 0.f, -1.f);
	glm::vec3 cameraUp = glm::vec3(0.f, 1.f, 0.f);
	//The camera rotation is represented in the order ZYX (Roll, Pitch , Yaw)
	glm::vec3 cameraRotation = glm::vec3(0.f, 0.f, 0.f);
	//The last position of the mouse on the X axis
	float lastX = 400.f;
	//The last position of the mouse on the y axis
	float lastY = 400.f;
	float cameraPitch = 0.f;
	float cameraYaw = 0.f;
	bool bHasCameraMoved = false;
	//-- FOV Variables --
	float cameraFov = 45.f;
	float originalCameraFov = 45.f;
	float cameraFovStep = 10.f;
	float minCameraFov = 20.f;
	float maxCameraFov = 120.f;

	float cameraSpeed = 5.f;
	float fastCameraSpeedMultiplier = 2.5f;
	float cameraSensitivity = 0.1f;

	//Camera functions
	void InitialiseCamera();

	//Calculates look at matrix according to given arguements
	glm::mat4 CalculateLookAtMatrix(glm::vec3 cameraPosition, glm::vec3 targetPosition, glm::vec3 worldUp);
	//
	void TranslateViewMatrix(glm::vec3 translateVector);
	//The camera rotation is represented in the order ZYX(Roll, Yaw, Pitch)
	void SetCameraRotation(glm::vec3 newCameraRotation);
	//Set front-vector of camera 
	void SetCameraUnitDirection(glm::vec3 newUnitDirection);
	//The camera rotation is represented in the order ZYX (Roll, Yaw, Pitch)
	void AddToCameraRotation(glm::vec3 rotationToAdd);
	void SetCameraPosition(glm::vec3 newCameraPosition);
	glm::vec3 GetCameraRotation() { return cameraRotation; };

	//Increases speed of camera movement
	void EnableFastCamera();
	//Returns speed of camera movement to default
	void DisableFastCamera();

	//Sets the fov of the camera, which updates the projection matrix
	void SetCameraFov(float newFov);
	//Get the camera's fov in float 
	float GetCameraFov() { return cameraFov; };

	GLFWwindow** windowPointer = nullptr;

};