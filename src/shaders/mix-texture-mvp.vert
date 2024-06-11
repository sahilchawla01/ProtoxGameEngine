#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 texCoord;

uniform mat4 mvp;

out vec3 newColor;
out vec2 uv; 

void main()
{
	//Transform position by Model-View-Projection matrix
	gl_Position = mvp * vec4(position, 1.0);
	newColor = color;
	uv = texCoord;
}

