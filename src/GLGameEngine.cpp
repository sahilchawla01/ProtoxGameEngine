#include "GLGameEngine.h"
#include <Shader.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


Game::Game()
{
	bShouldGameRun = true;
}

void Game::InputKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) 
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void Game::FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

double Game::GetTimeElapsedSinceLaunch()
{
	return TimeElapsed;
}

void Game::SetTimeElapsedSinceLaunch(int TimeElapsedSinceLaunch)
{
	TimeElapsed = TimeElapsedSinceLaunch;
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

	//Create the game object
	Game* game = new Game();

	//Set key callback which is called whenever a key is pressed
	glfwSetKeyCallback(window, game->InputKeyCallback);
	//Whenever frame buffer size changes, viewport size is changed
	glfwSetFramebufferSizeCallback(window, game->FrameBufferSizeCallback);

	//Create and setup color wheel shader
	Shader colorWheelShader("src/shaders/color-wheel.vert", "src/shaders/color-wheel.frag");
	//Setup inverted triangle shader
	Shader invertedColorWheelShader("src/shaders/vertically-invert.vert", "src/shaders/color-wheel.frag");
	//Setup x-offset shader
	Shader xAxisOffsetShader("src/shaders/xAxisUniformOffset.vert", "src/shaders/color-wheel.frag");
	//Create and setup simple texture shader
	Shader simpleTexShader("src/shaders/simple-texture.vert", "src/shaders/simple-texture.frag");

	//Create a rectangle object and setup vertex input
	float vertices[] = {
		//positions								//colors		//texture coordinate
		-0.5f, -0.5f, 0.f, /* Bottom left */ 1.f, 0.f, 0.f,		0.f, 0.f,
		0.5f, -0.5f, 0.f, /* Bottom Right*/ 0.f, 1.f, 0.f,		1.f, 0.f,
		-0.5f, 0.5f, 0.f, /* Top left*/ 0.f, 0.f, 1.f,			0.f, 1.f,
		0.5f, 0.5f, 0.f, /* Top Right*/ 1.f, 1.f, 0.f,			1.f, 1.f,

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

	//Create and bind a EBO(Element Buffer Object) to store indices to decide which vertices to draw
	unsigned int EBO;
	glGenBuffers(1, &EBO);

	//Bind the VAO first
	glBindVertexArray(VAO);

	//Bind the VBO, next
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Bind the EBO next
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//Finally, (Configure the vertex attribute) Tell OpenGl how to interpret vertex data 
	//Configure vertex data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//Configure color data
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//Configure texture coordinate
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//Unbind VBO, then unbind VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//Generate a texture object
	unsigned int containerTextureId;
	glGenTextures(1, &containerTextureId);
	//Bind texture to currently bound 2d Texture
	glBindTexture(GL_TEXTURE_2D, containerTextureId);

	//Set the texture filtering mode for wrapping, and scaling
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

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


	//Simple game logic:
		//Process input
		//Update
		//Render
	while (!glfwWindowShouldClose(window)) //Game loop
	{
		
		//Update:
		//Set elapsed time
		game->SetTimeElapsedSinceLaunch(static_cast<int>(glfwGetTime()));

		//If game should run is false, close the window
		if (game->bShouldGameRun == false) glfwSetWindowShouldClose(window, GLFW_TRUE);

		//Render:
		//Set background color
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Draw a rectangle
		// 
		//activate the shader program
		simpleTexShader.use();

		glBindTexture(GL_TEXTURE_2D, containerTextureId);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//glDrawArrays(GL_TRIANGLES, 0, 3);
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

