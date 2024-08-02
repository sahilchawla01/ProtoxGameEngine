#pragma once
#include "AActor.h"
#include <vector>
#include <string>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 texCoord;
};

struct Textures
{
	int id;
	std::string type;
};

class AMesh :  public AActor
{
public:
	std::vector<Vertex> vertices;
	std::vector<Textures> textures;
	std::vector<unsigned int> indices;

	AMesh(std::vector<Vertex> vertices, std::vector<Textures> textures, std::vector<unsigned int> indices);
	void Draw(Shader& shader);

private:
	//Render data
	unsigned int VAO, VBO, EBO;
	
	void SetupMesh();
};

