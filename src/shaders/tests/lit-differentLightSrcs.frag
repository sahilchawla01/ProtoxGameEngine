#version 330 core

//This shader accounts for light attenuation
//This shader accounts for different types of light sources

struct Material 
{
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shine;
};

struct Light {
    vec3 viewSpacePosition;
	vec3 worldPosition;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	//The light attenuation constants
	float constant;
    float linear;
    float quadratic;
};


uniform Material mat;
uniform Light light;

uniform vec3 objectColor;
//Redundant as camera view position in view space is 0,0,0
uniform vec3 cameraViewPosition;

in vec3 normal;
in vec3 FragViewPosition;
in vec3 FragWorldPosition;
in vec2 TexCoords;

out vec4 FragColor;

void main()
{

	// -- Calculate AMBIENT component of Phong --

	//Ambient becomes the same as diffuse's color
	vec3 ambient = light.ambient * vec3(texture(mat.diffuse, TexCoords));

	// -- Calculate DIFFUSE component of Phong -- 

	vec3 finalNormal = normalize(normal);
	vec3 lightDirection = normalize(light.viewSpacePosition - FragViewPosition);
	//Calculate dot product to get diffuse strength, and get max between 0 and dot product (to eradicate negative strength)
	float diff = max(dot(finalNormal, lightDirection), 0.0);
	//Final value for diffuse
	vec3 diffuse =  light.diffuse * diff * vec3(texture(mat.diffuse, TexCoords));


	// -- Calculate SPECULAR component of Phong -- 

	vec3 viewDirection = normalize(cameraViewPosition - FragViewPosition);
	vec3 reflectDir = reflect(-lightDirection, finalNormal);
	//Here, 32 is the "shinyness" value of the object i.e a property of the material 
	float spec = pow(max(dot(viewDirection, reflectDir), 0.0), mat.shine);
	//Finally calculate the specular vector
	vec3 specular =  light.specular * spec * vec3(texture(mat.specular, TexCoords));
	
	// -- Calculate Phong RESULTANT -- 
	vec3 phongResult = (ambient + diffuse + specular)  * objectColor;

	//-- Calculate ATTENUATION factor --
	float distance = length(light.worldPosition - FragWorldPosition);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	//Influence final lighting with attenuation
	phongResult *= attenuation;

	// -- EMISSION MAP --
	//vec3 emission = vec3(texture(mat.emission, TexCoords));
	//phongResult += emission;

	FragColor = vec4(phongResult, 1.0);
}