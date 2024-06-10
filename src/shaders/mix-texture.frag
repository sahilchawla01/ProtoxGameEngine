#version 330 core

in vec3 newColor;
in vec2 uv;

out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	// vec2 shiftedUv = vec2(uv.x + 1.0, uv.y);
	vec2 shiftedUv = uv * 2;
	FragColor = mix(texture(texture1, uv), texture(texture2, shiftedUv), 0.2);
}