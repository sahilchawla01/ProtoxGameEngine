#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

uniform mat4 mvp;
uniform mat4 modelMatrix;
uniform mat4 modelViewMatrix;
uniform vec3 lightPos;

out vec3 normal;
out vec3 FragViewSpacePosition;
out vec3 LightViewSpacePosition;

void main()
{
	//Transform position by Model-View-Projection matrix
	gl_Position = mvp * vec4(aPos, 1.0);
	
	//Store normal
	normal = mat3(transpose(inverse(modelViewMatrix))) * aNormal;
	
	//Send over view space position 
	FragViewSpacePosition = vec3(modelViewMatrix * vec4(aPos, 1.0));

	//Send over view space position for light
	LightViewSpacePosition = vec3(modelViewMatrix * vec4(lightPos, 1.0));

}

