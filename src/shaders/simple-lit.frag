#version 330 core

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewWorldPos;

in vec3 normal;
in vec3 FragWorldPosition;

out vec4 FragColor;

void main()
{

	//Calculate AMBIENT component of Phong
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * lightColor;

	// -- Calculate DIFFUSE component of Phong -- 
	vec3 finalNormal = normalize(normal);
	//vec3 finalNormal = normalize(normal);
	vec3 lightDirection = normalize(lightPos - FragWorldPosition);
	//Calculate dot product to get diffuse strength, and get max between 0 and dot product (to eradicate negative strength)
	float diff = max(dot(finalNormal, lightDirection), 0.0);
	//Final value for diffuse
	vec3 diffuse = diff * lightColor;

	//-- Calculate SPECULAR component of Phong -- 
	float specularStrength = 0.5f;
	vec3 viewDirection = normalize(viewWorldPos - FragWorldPosition);
	vec3 reflectDir = reflect(-lightDirection, finalNormal);
	//Here, 32 is the "shinyness" value of the object i.e a property of the material 
	float spec = pow(max(dot(viewDirection, reflectDir), 0.0), 32);
	//Finally calculate the specular vector
	vec3 specular = specularStrength * spec * lightColor;
	
	//Get lighting resultant and multiply with object colour
	vec3 phongResult = (ambient + diffuse + specular) * objectColor;

	FragColor = vec4(phongResult, 1.0);
}