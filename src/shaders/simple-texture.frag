#version 330 core

in vec3 newColor;
in vec2 uv;

out vec4 FragColor;

uniform sampler2D ourTexture;

void main()
{
	FragColor = texture(ourTexture, uv);
}