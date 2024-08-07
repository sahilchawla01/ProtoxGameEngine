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

struct DirectionalLight
{
	vec3 direction;

	//light's color values
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct SpotLight {
	vec3  worldPosition;
    vec3  direction;
    float cutOff;
	float outerCutOff;

	//Light color properties
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	//The light attenuation over distance constants
	float constant;
    float linear;
    float quadratic;
};

//-- Light uniforms -- 
//#define NR_POINT_LIGHTS 4

//uniform Light light[NR_POINT_LIGHTS];
uniform Light light;
uniform SpotLight flashLight;
uniform DirectionalLight dirLight;

uniform Material mat;

uniform vec3 objectColor;
//Redundant as camera view position in view space is 0,0,0 [ Only for learning purposes ]
uniform vec3 cameraViewPosition;

in vec3 normal;
in vec3 FragViewPosition;
in vec3 FragWorldPosition;
in vec2 TexCoords;

out vec4 FragColor;

vec3 CalculatePointLightPhong(Light pointLight)
{
	// -- Calculate AMBIENT component of Phong --

	//Ambient becomes the same as diffuse's color
	vec3 ambient = pointLight.ambient * vec3(texture(mat.diffuse, TexCoords));

	// -- Calculate DIFFUSE component of Phong -- 

	vec3 finalNormal = normalize(normal);
	vec3 lightDirection = normalize(pointLight.viewSpacePosition - FragViewPosition);
	//Calculate dot product to get diffuse strength, and get max between 0 and dot product (to eradicate negative strength)
	float diff = max(dot(finalNormal, lightDirection), 0.0);
	//Final value for diffuse
	vec3 diffuse =  pointLight.diffuse * diff * vec3(texture(mat.diffuse, TexCoords));


	// -- Calculate SPECULAR component of Phong -- 

	vec3 viewDirection = normalize(cameraViewPosition - FragViewPosition);
	vec3 reflectDir = reflect(-lightDirection, finalNormal);
	//Here, 32 is the "shinyness" value of the object i.e a property of the material 
	float spec = pow(max(dot(viewDirection, reflectDir), 0.0), mat.shine);
	//Finally calculate the specular vector
	vec3 specular =  pointLight.specular * spec * vec3(texture(mat.specular, TexCoords));
	
	// -- Calculate Phong RESULTANT -- 
	vec3 phongResult = (ambient + diffuse + specular)  * objectColor;

	//-- Calculate ATTENUATION factor --
	float distance = length(pointLight.worldPosition - FragWorldPosition);
	float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * (distance * distance));

	//Influence final lighting with attenuation
	phongResult *= attenuation;

	// -- EMISSION MAP --
	//vec3 emission = vec3(texture(mat.emission, TexCoords));
	//phongResult += emission;

	return phongResult;
}

vec3 CalculateSpotLightPhong(SpotLight spotLight)
{
	vec3 phongResult = vec3(0.f);
	vec3 lightDirection = normalize(spotLight.worldPosition - FragWorldPosition);

	// -- Calculate AMBIENT component of Phong --

	//Ambient becomes the same as diffuse's color
	vec3 ambient = spotLight.ambient * vec3(texture(mat.diffuse, TexCoords));
	//Ambient light is not affected by intensity

	// -- Calculate DIFFUSE component of Phong -- 

	vec3 finalNormal = normalize(normal);
	//Calculate dot product to get diffuse strength, and get max between 0 and dot product (to eradicate negative strength)
	float diff = max(dot(finalNormal, lightDirection), 0.0);
	//Final value for diffuse
	vec3 diffuse =  spotLight.diffuse * diff * vec3(texture(mat.diffuse, TexCoords));

	// -- Calculate SPECULAR component of Phong -- 

	vec3 viewDirection = normalize(cameraViewPosition - FragViewPosition);
	vec3 reflectDir = reflect(-lightDirection, finalNormal);
	//Here, 32 is the "shinyness" value of the object i.e a property of the material 
	float spec = pow(max(dot(viewDirection, reflectDir), 0.0), mat.shine);
	//Finally calculate the specular vector
	vec3 specular =  spotLight.specular * spec * vec3(texture(mat.specular, TexCoords));

	//This gives the cosine value (not the angle in degrees/radians), between the LightDirection and SpotLightDirection
	float theta = dot(lightDirection, normalize(-spotLight.direction));
	//Calculate light falloff within cuttoff cone
	float epsilon   = (spotLight.cutOff - spotLight.outerCutOff);
	float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0, 1.0);    

	//Influence diffuse component with spotlight intensity
	diffuse *= intensity;
	//Influence specular component with intensity
	specular *= intensity;
		
	// -- Calculate Phong RESULTANT -- 
	phongResult = (ambient + diffuse + specular)  * objectColor;

	// attenuation
    float distance  = length(spotLight.worldPosition - FragWorldPosition);
    float attenuation = 1.0 / (spotLight.constant + spotLight.linear * distance + spotLight.quadratic * (distance * distance));    

	//Influence phongResult with attenuation
	phongResult *= attenuation;

	return phongResult;
}

vec3 CalculateDirLightPhong(DirectionalLight dirLight)
{
	// -- Calculate AMBIENT component of Phong --

	//Ambient becomes the same as diffuse's color
	vec3 ambient = dirLight.ambient * vec3(texture(mat.diffuse, TexCoords));

	// -- Calculate DIFFUSE component of Phong -- 

	vec3 finalNormal = normalize(normal);
	vec3 lightDirection = normalize(-dirLight.direction);
	//Calculate dot product to get diffuse strength, and get max between 0 and dot product (to eradicate negative strength)
	float diff = max(dot(finalNormal, lightDirection), 0.0);
	//Final value for diffuse
	vec3 diffuse =  dirLight.diffuse * diff * vec3(texture(mat.diffuse, TexCoords));


	// -- Calculate SPECULAR component of Phong -- 

	vec3 viewDirection = normalize(cameraViewPosition - FragViewPosition);
	vec3 reflectDir = reflect(-lightDirection, finalNormal);
	//Here, 32 is the "shinyness" value of the object i.e a property of the material 
	float spec = pow(max(dot(viewDirection, reflectDir), 0.0), mat.shine);
	//Finally calculate the specular vector
	vec3 specular =  dirLight.specular * spec * vec3(texture(mat.specular, TexCoords));
	
	// -- Calculate Phong RESULTANT -- 
	vec3 phongResult = (ambient + diffuse + specular)  * objectColor;

	return phongResult;
}

void main()
{
	
	//--Step 1: Calculate influence of all point lights --
	vec3 pointLightPhongResult = CalculatePointLightPhong(light);
//	for(int i = 0; i < NR_POINT_LIGHTS; i++) //Iterate and add each point light's influence;
		 // pointLightPhongResult += CalculatePointLightPhong(light[i]);



	//-- Step 2: Get the spot light's influence (i.e flashlight as camera) --
	vec3 flashLightPhongResult = CalculateSpotLightPhong(flashLight);

	//-- Step 3: Get direction light's incluence --
	vec3 dirLightPhongResult = CalculateDirLightPhong(dirLight);

	vec3 totalPhongResult = pointLightPhongResult + flashLightPhongResult + dirLightPhongResult;

	FragColor = vec4(totalPhongResult, 1.0);
}