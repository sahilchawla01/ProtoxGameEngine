#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

uniform mat4 mvp;
uniform mat4 modelMatrix;

out vec3 normal;
out vec3 FragWorldPosition;

void main()
{
	//Transform position by Model-View-Projection matrix
	gl_Position = mvp * vec4(aPos, 1.0);
	
	//Store normal
	normal = mat3(transpose(inverse(modelMatrix))) * aNormal;
	
	//Send over model position (world pos)
	FragWorldPosition = vec3(modelMatrix * vec4(aPos, 1.0));

}

