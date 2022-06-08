#version 410 core

in vec3 texCoords;
out vec4 fragColor;

uniform samplerCube skybox;
uniform bool polygonMode;

void main()
{
	if (!polygonMode) fragColor = texture(skybox, texCoords);
}
