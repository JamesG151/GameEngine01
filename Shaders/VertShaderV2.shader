#version 450

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec4 tangent;

out vec2 vTexCoord;
out vec3 vNormal;
out vec3 vTangent;
out vec3 vBiTangent;
out vec4 vPosition;

uniform mat3 normalMatrix;

uniform mat4 projectionViewModel;

uniform mat4 modelMatrix;

void main()
{
	vTexCoord = texCoord;
	vPosition = modelMatrix * position;
	vNormal = (modelMatrix * vec4(normal.xyz, 0)).xyz;
	vTangent = normalMatrix * tangent.xyz;
	vBiTangent = cross(vNormal, vTangent) * tangent.w;

	gl_Position = projectionViewModel * position;
}