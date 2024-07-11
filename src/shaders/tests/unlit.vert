#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 mvp;

void main()
{
	//Transform position by Model-View-Projection matrix
	gl_Position = mvp * vec4(position, 1.0);
}

