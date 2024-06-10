#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

class Shader
{
	//The program ID
	unsigned int ID;

	//Constructor reads and builds the path
	Shader(const char* vertexPath, const char* fragmentPath);
	//Activate the shader
	void use();
	//Utility uniform functions
	void setBool(const std::string& name, bool value) const;
	void setFloat(const std::string& name, float value) const;
	void setInt(const std::string& name, int value) const;
};

#endif

