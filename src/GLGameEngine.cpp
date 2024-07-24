#include "GLGameEngine.h"
#include <Helpers/Shader.h>
#include <Helpers/LoadUtility.h>
#include <ThirdParty/imgui/imgui.h>
#include <ThirdParty/imgui/backends/imgui_impl_glfw.h>
#include <ThirdParty/imgui/backends/imgui_impl_opengl3.h>
#include <Actors/ACamera.h>
#include <vector>

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

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	// Our state
	bool show_demo_window = false;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


	//Set loader (GLAD)'s context to current window
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	//Enable depth testing so that triangles are not drawn in wrong order
	glEnable(GL_DEPTH_TEST);

	//Create the game object
	std::unique_ptr<Game>game = std::make_unique<Game>(&window);
	//Store pointer to game on window
	glfwSetWindowUserPointer(window, game.get());

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

	//Create a rectangle object and setup vertex input

	//This cube includes vertex, NORMAL, and texture coordinates for each side of the cube
	float cubeObjectVertices[] = {
	// positions          // normals           // texture coords
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

	// -- CREATE LIGHT OBJECT --
	glm::vec3 orgLightPos = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 currentLightPos = orgLightPos;
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Unbind VBO, then unbind VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//Create and setup unlit shader for lightobject
	Shader unlitShader("../../../src/shaders/tests/unlit.vert", "../../../src/shaders/tests/unlit.frag");

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

	std::vector<glm::vec3> cubePositions = {
	glm::vec3(0.0f,  1.0f,  0.0f),
	glm::vec3(1.0f,  0.0f, 0.0f),
	glm::vec3(-1.0f,  0.0f, 0.0f),
	glm::vec3(0.0f,  0.0f, -1.0f),
	glm::vec3(0.0f,  0.0f, 1.0f),
	glm::vec3(-1.0f,  0.0f, -1.0f),
	glm::vec3(-1.0f,  0.0f, 1.0f),
	glm::vec3(1.0f,  0.0f, -1.0f),
	glm::vec3(1.0f,  0.0f, 1.0f),
	};

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//normal vector attribute
	 glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//Unbind VBO, then unbind VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//Load the container texture to be used as diffuse texture
	unsigned int containerTextureId = LoadUtility::loadTexture("../../../assets/textures/container2.png");
	//Also load the specular texture
	unsigned int containerSpecularTexId = LoadUtility::loadTexture("../../../assets/textures/container2_specular.png");
	//Also load the emission texture
	unsigned int containerEmissionTexId = LoadUtility::loadTexture("../../../assets/textures/container2_emission.jpg");

	//Create and setup mixng texture shader
	Shader litShader("../../../src/shaders/tests/lit-differentLightSrcs.vert", "../../../src/shaders/tests/lit-differentLightSrcs.frag");

	//activate the shader program
	litShader.use();

	//Assign correct texture unit to the diffuse and specular uniform sampler
	litShader.setInt("mat.diffuse", 0); // 0th texture unit (corresponds to GLTEXTURE0) => DIFFUSE MAP
	litShader.setInt("mat.specular", 1); // 1st texture unit (corresponds to GLTEXTURE1) => SPECULAR MAP
	litShader.setInt("mat.emission", 2); // 1st texture unit (corresponds to GLTEXTURE2) => EMISSION MAP


	//Simple game logic:
	// 
	//Process input
	//Update
	//Render
	//

	// -- GAME LOOP -- 
	while (!glfwWindowShouldClose(window)) 
	{
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
		if(show_another_window){
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
			ImGui::End();
		}

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
			lightModelMatrix = glm::translate(lightModelMatrix, orgLightPos);
			////Also rotate light in XZ plane
			lightModelMatrix = glm::translate(lightModelMatrix, glm::vec3(cos(glfwGetTime()) * 4.f, sin(glfwGetTime()) + cos(glfwGetTime()), -sin(glfwGetTime()) * 4.f));
			lightModelMatrix = glm::scale(lightModelMatrix, glm::vec3(0.2f));

			//Get current model matrix's position
			currentLightPos = lightModelMatrix[3];

			//Get mvp matrix 
			glm::mat4 mvpMatrix = glm::mat4(1.f);
			mvpMatrix = game->GetProjectionMatrix() * game->GetViewMatrix() * lightModelMatrix;

			//Provide MVP matrix to the vertex shader
			unlitShader.setMat4("mvp", mvpMatrix);

			//Draw the light object
			glDrawArrays(GL_TRIANGLES, 0, 36);

		}
		// -- Start drawing all the cubes --

		//Activate shader
		litShader.use();

		// -- DRAW N CUBEs --
		{

			//Get current camera
			ACamera* currCamera = game->GetActiveCamera();
			glm::vec3 currentCamPosition = (currCamera) ? currCamera->GetCameraWorldPosition() : glm::vec3(0.f, 0.f, 0.f);

			//Provide object color and light color
			litShader.setVec3("objectColor", glm::vec3(1.f, 1.f, 1.f));
			//Transform camera world position to view position
			glm::vec3 cameraViewPosition = game->GetViewMatrix() * glm::vec4(currentCamPosition, 1.0);
			//Store positions
			litShader.setVec3("cameraViewPosition", cameraViewPosition);
			
			//-- Set light values --
			litShader.setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
			litShader.setVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
			litShader.setVec3("light.specular", glm::vec3(1.f, 1.f, 1.f));

			litShader.setVec3("light.worldPosition", currentLightPos);
			glm::vec3 lightViewPosition = game->GetViewMatrix() * glm::vec4(currentLightPos, 1.0);
			litShader.setVec3("light.viewSpacePosition", lightViewPosition);
			//Set attenuation constants
			litShader.setFloat("light.constant", 1.0f);
			litShader.setFloat("light.linear", 0.09f);
			litShader.setFloat("light.quadratic", 0.032f);
			 
			//--Set spotlight values--
			litShader.setVec3("flashLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
			litShader.setVec3("flashLight.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
			litShader.setVec3("flashLight.specular", glm::vec3(1.f, 1.f, 1.f));
			litShader.setVec3("flashLight.worldPosition", currentCamPosition);
			litShader.setVec3("flashLight.direction", currCamera->GetCameraForwardDirVector());
			litShader.setFloat("flashLight.cutOff", glm::cos(glm::radians(10.5f)));
			litShader.setFloat("flashLight.outerCutOff", glm::cos(glm::radians(13.5f)));
			//set distance falloff values
			litShader.setFloat("flashLight.constant", 1.0f);
			litShader.setFloat("flashLight.linear", 0.09f);
			litShader.setFloat("flashLight.quadratic", 0.032f);

			//--Set directional light values--
			litShader.setVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
			litShader.setVec3("dirLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
			litShader.setVec3("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
			litShader.setVec3("dirLight.specular", glm::vec3(0.6f, 0.6f, 0.6f));


			//Set material values
			litShader.setFloat("mat.shine", 32.0f);

			//Bind textures to corresponding texture units
			glActiveTexture(GL_TEXTURE0); //mat.diffuse was set to this value
			glBindTexture(GL_TEXTURE_2D, containerTextureId);
			glActiveTexture(GL_TEXTURE1); //mat.specular was set to this value
			glBindTexture(GL_TEXTURE_2D, containerSpecularTexId);
			glActiveTexture(GL_TEXTURE2); //mat.emission was set to this value
			glBindTexture(GL_TEXTURE_2D, containerEmissionTexId);

		
			for (int itr = 0; itr < cubePositions.size(); itr++)
			{
				
				glBindVertexArray(VAO);

				glm::mat4 cubeModelMatrix = glm::mat4(1.f);
				cubeModelMatrix = glm::translate(cubeModelMatrix, cubePositions[itr]);

				//Get mvp matrix 
				glm::mat4 mvpMatrix = glm::mat4(1.f);
				mvpMatrix = game->GetProjectionMatrix() * game->GetViewMatrix() * cubeModelMatrix;

				//Provide all transformations to the vertex shader
				litShader.setMat4("mvp", mvpMatrix);
				litShader.setMat4("modelViewMatrix", game->GetViewMatrix() * cubeModelMatrix);
				litShader.setMat4("modelMatrix", cubeModelMatrix);


				//Draw the light object
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}
	
		//
		//

		// GUI Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		//Swap buffers and poll input events
		glfwSwapBuffers(window);
		glfwPollEvents();


	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();


	//De-allocate all resources
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	//Destroy the window
	glfwDestroyWindow(window);
	//Terminate glfw before exiting
	glfwTerminate();
}

