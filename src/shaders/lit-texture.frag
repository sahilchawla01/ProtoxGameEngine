#version 330 core

in vec3 newColor;
in vec2 uv;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	vec4 textureValue = mix(texture(texture1, uv), texture(texture2, uv), 0.2);
	vec4 colorAffectedValue = vec4(lightColor, 1.0) * vec4(objectColor, 1.0) * textureValue;

	FragColor = colorAffectedValue;
}