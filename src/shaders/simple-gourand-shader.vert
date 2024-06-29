#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

uniform mat4 mvp;
uniform mat4 modelMatrix;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewWorldPos;

out vec3 phongResult;

void main()
{
	//Transform position by Model-View-Projection matrix
	gl_Position = mvp * vec4(aPos, 1.0);
	
	//Store normal
	vec3 normal = mat3(transpose(inverse(modelMatrix))) * aNormal;
	
	//Send over model position (world pos)
	vec3 FragWorldPosition = vec3(modelMatrix * vec4(aPos, 1.0));
	
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
	phongResult = (ambient + diffuse + specular) * objectColor;

}

