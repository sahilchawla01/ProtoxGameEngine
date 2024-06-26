#include "GLGameEngine.h"
#include <Shader.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <Actors/ACamera.h>


Game::Game(GLFWwindow** wndPtr)
{
	bShouldGameRun = true;

	//Store pointer to window
	windowPointer = wndPtr;

	InitialiseGame();
}

void Game::InputKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//Get game pointer
	Game* gamePtr = (Game*)glfwGetWindowUserPointer(window);

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);	
	//--  Handle fast camera -- 
	if(key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
	{
		if(gamePtr->activeCamera) gamePtr->activeCamera->EnableFastCamera();
		std::cout << "\nEnable fast camera";
	}
	else if(key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE)
	{
		if (gamePtr->activeCamera) gamePtr->activeCamera->DisableFastCamera();
		std::cout << "\nDisable fast camera";
	}
}

void Game::FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void Game::MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	//Get game pointer
	Game* gamePtr = (Game*)glfwGetWindowUserPointer(window);

	//If active camera exists, send the mouse input
	if (gamePtr->activeCamera) gamePtr->activeCamera->ProcessMouseInput(static_cast<float>(xpos), static_cast<float>(ypos));

	////If mouse hasn't moved since start of the window
	//if (!gamePtr->bHasCameraMoved)
	//{
	//	gamePtr->lastX = xpos;
	//	gamePtr->lastY = ypos;
	//	gamePtr->bHasCameraMoved = true;
	//}
	////Calculate offset since previous
	//float xOffset = xpos - gamePtr->lastX;
	//float yOffset = gamePtr->lastY - ypos;
	////Store last x and y values
	//gamePtr->lastX = xpos;
	//gamePtr->lastY = ypos;

	////Factor in camera sensitivity
	//xOffset *= gamePtr->cameraSensitivity;
	//yOffset *= gamePtr->cameraSensitivity;

	////Add offset values to camera's rotation
	//gamePtr->cameraPitch += yOffset;
	//gamePtr->cameraYaw += xOffset;

	////Constraint looking up and down beyond 89 degrees
	//if (gamePtr->cameraPitch > 89.f) gamePtr->cameraPitch = 89.f;
	//else if (gamePtr->cameraPitch < -89.f) gamePtr->cameraPitch = -89.f;

	////Calculate direction vector that gamePtr->camera now looks at 
	//glm::vec3 direction(0.f, 0.f, 0.f);
	//direction.x = cos(glm::radians(gamePtr->cameraYaw)) * cos(glm::radians(gamePtr->cameraPitch));
	//direction.y = sin(glm::radians(gamePtr->cameraPitch));
	//direction.z = sin(glm::radians(gamePtr->cameraYaw)) * cos(glm::radians(gamePtr->cameraPitch));
	//direction = glm::normalize(direction);

	////Set camera's new unit direction
	//gamePtr->SetCameraUnitDirection(direction);
	////Set camera rotation
	//gamePtr->SetCameraRotation(glm::vec3(gamePtr->cameraPitch, gamePtr->cameraYaw, 0.f));

}

void Game::ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	//Get game pointer
	Game* gamePtr = (Game*)glfwGetWindowUserPointer(window);

	////Positive implies zoom in, negative implies zoom out
	//float zoomDirectionScale = 1.f;
	////If zooming out, invert zoom direction
	//if (yOffset < 0.0) zoomDirectionScale = -1.f;

	//float currCameraFov = gamePtr->GetCameraFov();
	////Finally, set camera fov
	//gamePtr->SetCameraFov(currCameraFov - (gamePtr->cameraFovStep * zoomDirectionScale));

	//Set the currently active camera's FOV
	if(gamePtr->activeCamera) gamePtr->activeCamera->ProcessScrollInput(static_cast<float>(xOffset), static_cast<float>(yOffset));
}

double Game::GetTimeElapsedSinceLaunch()
{
	return TimeElapsed;
}

void Game::SetTimeElapsedSinceLaunch(int TimeElapsedSinceLaunch)
{
	TimeElapsed = TimeElapsedSinceLaunch;
}

void Game::ProcessInput(GLFWwindow* window)
{

	//-- Handle WASD movement for camera  --
	if (activeCamera) 
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			activeCamera->ProcessKeyboardInput(ECameraMoveDirection::FORWARD, deltaTime);
			/*glm::vec3 newCamPos = currentCameraPosition + (cameraSpeed * deltaTime) * cameraFront;
			SetCameraPosition(newCamPos);*/
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			activeCamera->ProcessKeyboardInput(ECameraMoveDirection::BACKWARD, deltaTime);
			/*glm::vec3 newCamPos = currentCameraPosition - (cameraSpeed * deltaTime) * cameraFront;
			SetCameraPosition(newCamPos);*/
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			activeCamera->ProcessKeyboardInput(ECameraMoveDirection::LEFT, deltaTime);
			/*glm::vec3 newCamPos = currentCameraPosition - glm::normalize(glm::cross(cameraFront, cameraUp)) * (cameraSpeed * deltaTime);
			SetCameraPosition(newCamPos);*/
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			activeCamera->ProcessKeyboardInput(ECameraMoveDirection::RIGHT, deltaTime);
			/*glm::vec3 newCamPos = currentCameraPosition + glm::normalize(glm::cross(cameraFront, cameraUp)) * (cameraSpeed * deltaTime);
			SetCameraPosition(newCamPos);*/
		}

	}
}

void Game::InitialiseGame()
{
	//Store window size 
	glfwGetWindowSize(*windowPointer, &windowWidth, &windowHeight);

	float aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);

	//Create a "editor" camera
	activeCamera = std::make_unique<ACamera>(glm::vec3(0.0f, 0.0f, 10.f), glm::vec3(0.f, 1.f, 0.f), -90.f, 0.f, aspectRatio);
}


void Game::SetDeltaTime(float DeltaTime)
{
	deltaTime = DeltaTime;
}

void Game::SetLastFrameTime(float LastFrameTime)
{
	lastFrame = LastFrameTime;
}

glm::mat4 Game::GetProjectionMatrix()
{
	if (!activeCamera)
	{
		std::cout << "\nError! Active camera was null when getting projection matrix";
		return glm::mat4(1.f);
	}

	return activeCamera->GetProjectionMatrix();
}

glm::mat4 Game::GetViewMatrix()
{
	if (!activeCamera)
	{
		std::cout << "\nError! Active camera was null when getting view matrix";
		return glm::mat4(1.f);
	}

	return activeCamera->GetViewMatrix();
}

ACamera* Game::GetActiveCamera()
{
	return activeCamera.get();
}


void ErrorCallback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

int main()
{
	if (!glfwInit())
	{
		//Initialistion failed
		std::cout << "GLFW Initialisation failed" << std::endl;
		return 0;
	}

	//Initialise error callback incase any glfw error arises
	glfwSetErrorCallback(ErrorCallback);

	//Window hint sets the minimum OpenGl version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Create the window and set the opengl context
	GLFWwindow* window = glfwCreateWindow(640, 480, "Protox Game Engine", NULL, NULL);
	if (!window)
	{
		//Window failed to create
		std::cout << "\nFailed to create glfw window\n";
		return 0;
	}

	//The current open GL context is set to the window
	glfwMakeContextCurrent(window);

	//Set loader (GLAD)'s context to current window
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	//Enable depth testing so that triangles are not drawn in wrong order
	glEnable(GL_DEPTH_TEST);

	//Create the game object
	Game* game = new Game(&window);
	//Store pointer to game on window
	glfwSetWindowUserPointer(window, game);

	//Enable mouse 
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//Set key callback which is called whenever a key is pressed
	glfwSetKeyCallback(window, Game::InputKeyCallback);
	//Set mouse callback
	glfwSetCursorPosCallback(window, Game::MouseCallback);
	//Set scroll callback
	glfwSetScrollCallback(window, Game::ScrollCallback);
	//Whenever frame buffer size changes, viewport size is changed
	glfwSetFramebufferSizeCallback(window, game->FrameBufferSizeCallback);

	//Create and setup mixng texture shader
	Shader litShader("src/shaders/simple-lit.vert", "src/shaders/simple-lit.frag");

	//Create a rectangle object and setup vertex input
	//float cubeObjectVertices[] = {
	//	//positions								//colors		//texture coordinate
	//	-0.5f, -0.5f, 0.f, /* Bottom left */ 1.f, 0.f, 0.f,		0.f, 0.f,
	//	0.5f, -0.5f, 0.f, /* Bottom Right*/ 0.f, 1.f, 0.f,		1.f, 0.f,
	//	-0.5f, 0.5f, 0.f, /* Top left*/ 0.f, 0.f, 1.f,			0.f, 1.f,
	//	0.5f, 0.5f, 0.f, /* Top Right*/ 1.f, 1.f, 0.f,			1.f, 1.f,

	//};
	// 
	//3 dimensional cube

	//This cube includes vertex and UV coordinates
	/*float cubeObjectVertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};*/

	//This cube includes vertex and NORMAL coordinates for each side of the cube
	float cubeObjectVertices[] =
	{
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	// -- CREATE LIGHT OBJECT --
	glm::vec3 lightPosition = glm::vec3(0.f, 1.0f, 4.f);
	glm::vec3 lightColor = glm::vec3(1.f, 1.f, 1.f);

	//Create a VAO
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);

	//Create a VBO
	unsigned int lightVBO;
	glGenBuffers(1, &lightVBO);

	//Bind the VAO first
	glBindVertexArray(lightVAO);

	//Bind the VBO with a cube's vertices
	glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeObjectVertices), cubeObjectVertices, GL_STATIC_DRAW);

	//Instruct the VBO how to interpret the data
	//vertex coord attrb
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Unbind VBO, then unbind VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//Create and setup unlit shader for lightobject
	Shader unlitShader("src/shaders/unlit.vert", "src/shaders/unlit.frag");

	//activate the shader program
	unlitShader.use();

	//Multiple cubes positions in the world
	/*glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
	};*/

	unsigned int indices[] =
	{
		//First triangle
		0, 1, 2,
		//Second triangle
		1, 2, 3
	};

	//Create and bind a VAO(Vertex Array Object) that stores pointers to VBOS for the corresponding object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	//Create and bind a VBO(Vertex Buffer Object) to send vertex data to GPU
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	//Create and bind a EBO(Element Buffer Object) to store indices to decide which cubeObjectVertices to draw
	unsigned int EBO;
	glGenBuffers(1, &EBO);

	//Bind the VAO first
	glBindVertexArray(VAO);

	//Bind the VBO, next
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeObjectVertices), cubeObjectVertices, GL_STATIC_DRAW);

	//Bind the EBO next
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//Finally, (Configure the vertex attribute) Tell OpenGl how to interpret vertex data 
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//normal vector attribute
	 glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	//Unbind VBO, then unbind VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//Generate a first texture object
	unsigned int containerTextureId;
	glGenTextures(1, &containerTextureId);
	//Activate texture unit (texture location) before binding texture
	glActiveTexture(GL_TEXTURE0);
	//Bind texture to currently bound 2d Texture
	glBindTexture(GL_TEXTURE_2D, containerTextureId);

	//Set the texture filtering mode for wrapping, and scaling
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//Flip y axis for images
	stbi_set_flip_vertically_on_load(true);

	//Load a sample texture
	int width, height, nrChannels;
	unsigned char* data = stbi_load("assets/textures/container.jpg", &width, &height, &nrChannels, 0);

	if (data)
	{
		//Bind data (of texture) to GL_TEXTURE_2D
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		//Generate the mipmaps 
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "\nError, couldn't load container.jpg" << std::endl;
	}
	//Free image data
	stbi_image_free(data);

	//Generate a second texture object
	unsigned int smileTexId;
	glGenTextures(1, &smileTexId);
	//Activate texture unit (texture location) before binding texture
	glActiveTexture(GL_TEXTURE1);
	//Bind texture to currently bound 2d Texture
	glBindTexture(GL_TEXTURE_2D, smileTexId);

	//Set the texture filtering mode for wrapping, and scaling
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//Load a second texture
	unsigned char* smileTexData = stbi_load("assets/textures/awesomeface.png", &width, &height, &nrChannels, 0);

	if (smileTexData)
	{
		//Bind smileTexData (of texture) to GL_TEXTURE_2D
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, smileTexData);
		//Generate the mipmaps 
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "\nError, couldn't load awesomeface.png" << std::endl;
	}
	//Free image smileTexData
	stbi_image_free(smileTexData);

	//activate the shader program
	litShader.use();
	//Set light position 
	litShader.setVec3("lightPos", lightPosition);
	//Set texture unit values
	/*litShader.setInt("texture1", 0);
	litShader.setInt("texture2", 1);*/
	

	

	//Simple game logic:
		//Process input
		//Update
		//Render
	// -- GAME LOOP -- 
	while (!glfwWindowShouldClose(window)) 
	{

		//Input
		//----------
		game->ProcessInput(window);
		
		// -- Update --
		//Set elapsed time and delta time
		float currentTime = static_cast<float>(glfwGetTime());
		game->SetTimeElapsedSinceLaunch(static_cast<int>(currentTime));
		game->SetDeltaTime(currentTime - game->GetLastFrameTime());
		game->SetLastFrameTime(currentTime);

		//If game should run is false, close the window
		if (game->bShouldGameRun == false) glfwSetWindowShouldClose(window, GLFW_TRUE);

		//Render:
		//Set background color
		glClearColor(0.f, 0.f, 0.f, 1.0f);
		//clear color and depth buffer so last frame's buffers aren't carried over
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// -- Draw light object --
		{
			unlitShader.use();

			glBindVertexArray(lightVAO);
		
			glm::mat4 lightModelMatrix = glm::mat4(1.f);
			lightModelMatrix = glm::translate(lightModelMatrix, lightPosition);
			lightModelMatrix = glm::scale(lightModelMatrix, glm::vec3(0.2f));

			//Get mvp matrix 
			glm::mat4 mvpMatrix = glm::mat4(1.f);
			mvpMatrix = game->GetProjectionMatrix() * game->GetViewMatrix() * lightModelMatrix;

			//Provide MVP matrix to the vertex shader
			unlitShader.setMat4("mvp", mvpMatrix);

			//Draw the light object
			glDrawArrays(GL_TRIANGLES, 0, 36);

		}
		// -- Start drawing all the cubes --

		//Bind textures to corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, containerTextureId);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, smileTexId);

		//Activate shader
		litShader.use();

		// -- DRAW 1 CUBE --
		{
			glBindVertexArray(VAO);

			glm::mat4 cubeModelMatrix = glm::mat4(1.f);

			//Get mvp matrix 
			glm::mat4 mvpMatrix = glm::mat4(1.f);
			mvpMatrix = game->GetProjectionMatrix() * game->GetViewMatrix() * cubeModelMatrix;

			//Get current camera
			ACamera* currCamera = game->GetActiveCamera();
			glm::vec3 currentCamPosition = (currCamera) ? currCamera->currentCameraPosition : glm::vec3(0.f, 0.f, 0.f);

			//Provide MVP matrix to the vertex shader
			litShader.setMat4("mvp", mvpMatrix);	
			//Provide model matrix to vertex shader
			litShader.setMat4("modelMatrix", cubeModelMatrix);
			//Provide object color and light color
			litShader.setVec3("objectColor", glm::vec3(1.f, 1.f, 1.f));
			litShader.setVec3("lightColor", lightColor);
			//Store positions
			litShader.setVec3("viewWorldPos", currentCamPosition);
			litShader.setVec3("lightPos", lightPosition);

			//Draw the light object
			glDrawArrays(GL_TRIANGLES, 0, 36);

		}

		// -- DRAW 10 CUBES (COMMENTED) --
		/*{
			glm::mat4 model = glm::mat4(1.f);
			model = glm::rotate(model, glm::radians((float)glfwGetTime() * 50.f), glm::vec3(1.f, 0.5f, 0.f));

			//Draw textured cube
			glBindVertexArray(VAO);

			//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			for (int i = 0; i < 10; i++)
			{
				//Change object position through model matrix
				model = glm::mat4(1.f);
				model = glm::translate(model, cubePositions[i]);
				float angle = 10.f * i;

				if (i == 0) angle = 20.f;

				if (i != 0) model = glm::rotate(model, glm::radians(angle * (float)glfwGetTime()), glm::vec3(0.3f, 1.f, 0.5f));

				//Change view matrix
				//const float radius = 10.0f;
				//float camX = sin(glfwGetTime()) * radius;
				//float camZ = cos(glfwGetTime()) * radius;
				//game->SetViewMatrix(glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0)));

				//Create MVP matrix 
				glm::mat4 mvpMatrix = glm::mat4(1.f);
				mvpMatrix = game->GetProjectionMatrix() * game->GetViewMatrix() * model;

				unsigned int mvpUniLocation = glGetUniformLocation(litShader.ID, "mvp");
				//Provide MVP matrix to the vertex shader
				glUniformMatrix4fv(mvpUniLocation, 1, GL_FALSE, glm::value_ptr(mvpMatrix));

				//Sendobject and light color to frag shader
				litShader.setVec3("objectColor", glm::vec3(0.5f, 0.5f, 1.f));
				litShader.setVec3("lightColor", lightColor);


				//Draw the triangle
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
			
		}
	*/
	
		//
		//


		//Swap buffers and poll input events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//De-allocate all resources
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	//Destroy the window
	glfwDestroyWindow(window);
	//Terminate glfw before exiting
	glfwTerminate();
}

