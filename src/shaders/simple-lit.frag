#version 330 core

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

in vec3 normal;
in vec3 FragWorldPosition;

out vec4 FragColor;

void main()
{
	float ambientStrength = 0.1f;

	//Calculate DIFFUSE component of Phong
	vec3 ambient = ambientStrength * lightColor;

	// -- Calculate DIFFUSE component of Phong -- 
	vec3 finalNormal = normalize(normal);
	vec3 lightDirection = normalize(lightPos - FragWorldPosition);

	//Calculate dot product to get diffuse strength, and get max between 0 and dot product (to eradicate negative strength)
	float diffuse = max(dot(lightDirection, finalNormal), 0.0);
	
	//Get lighting resultant and multiply with object colour
	vec3 phongResult = (ambient + diffuse) * objectColor;

	FragColor = vec4(phongResult, 1.0);
}