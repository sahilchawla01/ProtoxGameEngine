#version 330

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 newColor;


void main()
{
	gl_Position = vec4(aPos.x, aPos.y * -1, aPos.z, 1.0);
	newColor = aColor;
}
