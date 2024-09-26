#pragma once
#include <vector>
#include <string>
#include <ThirdParty/glm/glm.hpp>
#include <ThirdParty/glm/gtc/matrix_transform.hpp>
#include <ThirdParty/glm/gtc/type_ptr.hpp>
#include <Helpers/Shader.h>
#include <Helpers/LoadUtility.h>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 texCoord;
};

struct Texture
{
	int id;
	std::string type;
};

class AMesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<Texture> textures;
	std::vector<unsigned int> indices;

	AMesh(std::vector<Vertex> vertices, std::vector<Texture> textures, std::vector<unsigned int> indices);
	void Draw(Shader& shader);

private:
	//Render data
	unsigned int VAO, VBO, EBO;
	
	void SetupMesh();
};

