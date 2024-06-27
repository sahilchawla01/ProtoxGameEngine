#version 330 core

uniform vec3 objectColor;
uniform vec3 lightColor;

out vec4 FragColor;

void main()
{
	float ambientStrength = 0.1f;

	vec3 ambient = ambientStrength * lightColor;

	vec3 phongResult = ambient * objectColor;

	FragColor = vec4(phongResult, 1.0);
}