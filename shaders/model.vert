#version 410 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoords;
layout (location = 3) in vec3 inTangent;
layout (location = 4) in vec3 inBiTangent;

uniform mat4 model;

out vec3 WorldPos_CS_in;
out vec2 TexCoord_CS_in;
out vec3 Normal_CS_in;
out mat3 TBN;

void main()
{
	WorldPos_CS_in = (model * vec4(inPos, 1.0)).xyz;
	TexCoord_CS_in = inTexCoords;
	Normal_CS_in   = (model * vec4(inNormal, 0.0)).xyz;

	vec3 T = normalize((model*vec4(inTangent, 0.0f)).xyz);
	vec3 B = normalize((model*vec4(inBiTangent, 0.0f)).xyz);
	vec3 N = normalize((model*vec4(inNormal, 0.0f)).xyz);
	TBN = mat3(T,B,N);
}