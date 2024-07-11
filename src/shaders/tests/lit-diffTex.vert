#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 mvp;
uniform mat4 modelViewMatrix;

out vec3 normal;
out vec3 FragViewPosition;
out vec2 TexCoords;

void main()
{
	//Transform position by Model-View-Projection matrix
	gl_Position = mvp * vec4(aPos, 1.0);
	
	//Store normal
	normal = mat3(transpose(inverse(modelViewMatrix))) * aNormal;
	
	//Send over model position (world pos)
	FragViewPosition = vec3(modelViewMatrix * vec4(aPos, 1.0));

	//Send tex coords to frag
	TexCoords = aTexCoords;

}

